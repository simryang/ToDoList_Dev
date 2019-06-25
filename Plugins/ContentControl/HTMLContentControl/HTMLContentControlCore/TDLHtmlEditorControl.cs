﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

using Abstractspoon.Tdl.PluginHelpers;

namespace HTMLContentControl
{
	delegate void NeedLinkTooltipEventHandler(object sender, NeedLinkTooltipEventArgs args);

	public class NeedLinkTooltipEventArgs : EventArgs
	{
		public NeedLinkTooltipEventArgs(String href) { linkUri = href; }

		public String linkUri;
		public String tooltip;
	}

	[System.ComponentModel.DesignerCategory("")]
    class TDLHtmlEditorControl : HtmlEditorControlEx //MSDN.Html.Editor.HtmlEditorControl
    {
        private Timer m_TextChangeTimer;
        private String m_PrevTextChange = "";
		private Boolean m_SettingContent = false;
		private String m_CurrentHRef = "";

        // ---------------------------------------------------------------

        public new event EventHandler TextChanged;
		public new event MSDN.Html.Editor.HtmlNavigationEventHandler HtmlNavigation;
		public event NeedLinkTooltipEventHandler NeedLinkTooltip;

		// ---------------------------------------------------------------

		public TDLHtmlEditorControl(System.Drawing.Font font, Translator trans) : base(font, trans)
        {
            m_TextChangeTimer = new Timer();

            InitializeComponent();
        }

		private void InitializeComponent()
        {
			base.InitializeComponentEx();

            this.ToolbarDock = DockStyle.Top;
            this.Anchor = AnchorStyles.Left | AnchorStyles.Top | AnchorStyles.Right | AnchorStyles.Bottom;
            this.InnerText = "";
            this.TabIndex = 26;
            this.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.BorderSize = 0;
            this.NavigateAction = MSDN.Html.Editor.NavigateActionOption.NewWindow;
			this.ContentMargin = 8;

			this.BrowserPanel.Anchor = AnchorStyles.None; // we handle positioning ourselves

			InitialiseFeatures();

            m_TextChangeTimer.Tick += new EventHandler(OnTextChangeTimer);
            m_TextChangeTimer.Interval = 200;
            m_TextChangeTimer.Start();

			// Dynamic tooltip handling
			this.WebBrowser.Document.MouseOver += new HtmlElementEventHandler(OnDocumentMouseOver);

			// Handle link clicking
			this.WebBrowser.Document.MouseDown += new HtmlElementEventHandler(OnDocumentMouseDown);
			this.WebBrowser.Document.MouseUp += new HtmlElementEventHandler(OnDocumentMouseUp);

			base.HtmlNavigation += new MSDN.Html.Editor.HtmlNavigationEventHandler(OnBaseNavigateLink);

		}

		private void InitialiseFeatures()
        {
            // remove whole 'Document' submenu
			base.InitialiseFeatures(true);
		}

		private void OnTextChangeTimer(object sender, EventArgs e)
        {
            if (!IsDisposed)
            {
                var s = InnerHtml ?? string.Empty;
                var p = m_PrevTextChange ?? string.Empty;

                if ((p.Length != s.Length) || (p != s))
                {
                    m_PrevTextChange = s;

                    if (!m_SettingContent && !IsLoading && !IsNavigating && (TextChanged != null))
                    {
                        TextChanged(this, new EventArgs());
                    }
                }
            }
        }

		public Byte[] GetContent()
        {
            var html = InnerHtml ?? String.Empty;

            return System.Text.Encoding.Unicode.GetBytes(html);
        }

        public bool SetContent(Byte[] content, bool bResetSelection)
        {
            m_SettingContent = true;

            try
            {
                var html = System.Text.Encoding.Unicode.GetString(content).TrimEnd('\0').Trim();

                InnerHtml = html;
                m_PrevTextChange = InnerHtml;
            }
            // catch (Exception exception)
            // {
            // }
            finally
            {
                m_SettingContent = false;
            }

            return true;
        }

        // text content if supported. return false if not supported
        public String GetTextContent()
        {
            return InnerText ?? String.Empty;
        }

        public bool SetTextContent(String content, bool bResetSelection)
        {
            m_SettingContent = true;

            try
            {
                InnerText = content;
                m_PrevTextChange = InnerHtml;
            }
            // catch (Exception exception)
            // {
            // }
            finally
            {
                m_SettingContent = false;
            }

            return true;
        }

		public bool InsertTextContent(String content, bool bAtEnd)
		{
			if (bAtEnd)
				WebBrowser.Document.Body.OuterText += content;

			// else
			SelectedText = content;
			return true;
		}

		protected override void PreShowDialog(Form dialog)
        {
            base.PreShowDialog(dialog, HTMLContentControlCore.html);
		}

		public bool ProcessMessage(IntPtr hwnd, UInt32 message, UInt32 wParam, UInt32 lParam, UInt32 time, Int32 xPos, Int32 yPos)
        {
            // Handle keyboard shortcuts
            if ((message == 0x0100) || (message == 0x0104) && (Control.ModifierKeys != Keys.None))
            {
                Keys keyPress = (Keys)wParam;

                if (keyPress == Keys.ControlKey)
                    return false;
                
                var modifiers = Control.ModifierKeys;

                if ((modifiers & Keys.Control) == Keys.Control)
                    keyPress |= Keys.Control;

                if ((modifiers & Keys.Shift) == Keys.Shift)
                    keyPress |= Keys.Shift;

                if ((modifiers & Keys.Alt) == Keys.Alt)
                    keyPress |= Keys.Alt;

                return CommandHandling.ProcessMenuShortcut(keyPress, ContextMenu.Items);
            }

            return false;
        }

		override protected bool IsValidHref(string href)
		{
			Uri unused;

			if (Uri.TryCreate(href, UriKind.Absolute, out unused))
				return true;

			return href.ToLower().StartsWith("outlook://");
		}

		private void OnDocumentMouseOver(object sender, HtmlElementEventArgs e)
		{
			var element = e.ToElement;

			if ((element != null) && (element.TagName.Equals("A")))
			{
				String href = element.GetAttribute("href"), tooltip = href;

				if (NeedLinkTooltip != null)
				{
					var args = new NeedLinkTooltipEventArgs(href);
					NeedLinkTooltip(this, args);

					if (!String.IsNullOrWhiteSpace(args.tooltip))
						tooltip = args.tooltip;
				}

				if (!tooltip.Equals(element.GetAttribute("title")))
				{
					// Prevent setting the tooltip causing a text change notification
					m_TextChangeTimer.Stop();

					element.SetAttribute("title", tooltip);
					m_PrevTextChange = InnerHtml;

					m_TextChangeTimer.Start();
				}
			}
		}

		private void OnDocumentMouseDown(object sender, HtmlElementEventArgs e)
		{
			if (!IsEditable)
			{
				// We handle links ourselves because the web browser 
				// doesn't always send a navigation notification
				// ie. It silently eats urls it doesn't like
				var element = this.WebBrowser.Document.GetElementFromPoint(e.ClientMousePosition);

				if (element == null)
					return;

				m_CurrentHRef = element.GetAttribute("href");
			}
		}

		private void OnDocumentMouseUp(object sender, HtmlElementEventArgs e)
		{
			if (!IsEditable && !String.IsNullOrWhiteSpace(m_CurrentHRef))
			{
				var href = new StringBuilder(m_CurrentHRef).ToString();
				m_CurrentHRef = "";

				// Verify that the mouse hasn't moved
				var element = this.WebBrowser.Document.GetElementFromPoint(e.ClientMousePosition);

				if (element == null)
					return;

				if (!href.Equals(element.GetAttribute("href")))
					return;

				if (HtmlNavigation != null)
				{
					HtmlNavigation(this, new MSDN.Html.Editor.HtmlNavigationEventArgs(href));
				}
			}
		}

		private void OnBaseNavigateLink(object sender, MSDN.Html.Editor.HtmlNavigationEventArgs e)
		{
			if (e.Url.ToLower() == "about:blank")
			{
				e.Cancel = false; // allow
			}
			else
			{
				e.Cancel = true; // everything else

				if (IsEditable) // Must be drag and drop
				{
					bool isImage = IsValidImageHref(e.Url);
					var rng = GetTextRange();

					if (rng == null)
						return;

					// Get the element under the mouse and move the select
					// before or after whichever is closer
					var pos = WebBrowser.PointToClient(Cursor.Position);
					var element = this.WebBrowser.Document.GetElementFromPoint(pos);

					if (element == null)
						return;

					// Create a new element after
					var newElm = this.WebBrowser.Document.CreateElement("span");

					if (newElm == null)
						return;

					newElm.InnerText = (' ' + (isImage ? "." : e.Url) + ' ');

					if (element.TagName == "BODY")
						element.AppendChild(newElm);
					else
						element.InsertAdjacentElement(HtmlElementInsertionOrientation.AfterEnd, newElm);

					rng.moveToElementText(newElm.DomElement as mshtml.IHTMLElement);
					rng.select();

					bool success = false;

					if (isImage)
						success = InsertImage(e.Url, "", MSDN.Html.Editor.ImageAlignOption.Default);
					else
						success = InsertLinkPrompt(e.Url);

					if (!success)
					{
						newElm.OuterHtml = "";
						m_PrevTextChange = InnerHtml; // prevent change
					}
				}
			}
		}

	}
}
