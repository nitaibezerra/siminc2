<?php
class HtmlMimeMail {
	public $headers;
	public $body;
	public $multipart;
	public $mime;
	public $html;
	public $html_text;
	public $html_images = array();
	public $cids = array();
	public $do_html;
	public $parts = array();

	/***************************************
	** Constructor function. Sets the headers
	** if supplied.
	***************************************/

	function HtmlMimeMail($headers = '') {
		$this->headers = $headers;
		}
	/***************************************
	** Adds a html part to the mail.
	** Also replaces image names with
	** content-id's.
	***************************************/

	function add_html($html, $text) {
		$this->do_html = 1;
		$this->html = $html;
		$this->html_text = $text;

		if (is_array($this->html_images) AND count($this->html_images) > 0) {
			for($i=0; $i<count($this->html_images); $i++) {
				$this->html = ereg_replace($this->html_images[$i]['name'], 'cid:'.$this->html_images[$i]['cid'], $this->html);
				}
			}
		}

	/***************************************
	** Builds html part of email.
	***************************************/
	function build_html($orig_boundary) {
		$sec_boundary = '=_'.md5(uniqid(time()));
		$thr_boundary = '=_'.md5(uniqid(time()));

		if (!is_array($this->html_images)) {
			$this->multipart.= '--'.$orig_boundary."\n";
			$this->multipart.= 'Content-Type: multipart/alternative; boundary = "'.$sec_boundary."\"\n\n\n";
			$this->multipart.= '--'.$sec_boundary."\n";
			$this->multipart.= 'Content-Type: text/plain'."\n";
			$this->multipart.= 'Content-Transfer-Encoding: 7bit'."\n\n";
			$this->multipart.= $this->html_text."\n\n";
			$this->multipart.= '--'.$sec_boundary."\n";
			$this->multipart.= 'Content-Type: text/html'."\n";
			$this->multipart.= 'Content-Transfer-Encoding: 7bit'."\n\n";
			$this->multipart.= $this->html."\n\n";
			$this->multipart.= '--'.$sec_boundary."--\n\n";
			}
		else {
			$this->multipart.= '--'.$orig_boundary."\n";
			$this->multipart.= 'Content-Type: multipart/related; boundary = "'.$sec_boundary."\"\n\n\n";
			$this->multipart.= '--'.$sec_boundary."\n";
			$this->multipart.= 'Content-Type: multipart/alternative; boundary = "'.$thr_boundary."\"\n\n\n";
			$this->multipart.= '--'.$thr_boundary."\n";
			$this->multipart.= 'Content-Type: text/plain'."\n";
			$this->multipart.= 'Content-Transfer-Encoding: 7bit'."\n\n";
			$this->multipart.= $this->html_text."\n\n";
			$this->multipart.= '--'.$thr_boundary."\n";
			$this->multipart.= 'Content-Type: text/html'."\n";
			$this->multipart.= 'Content-Transfer-Encoding: 7bit'."\n\n";
			$this->multipart.= $this->html."\n\n";
			$this->multipart.= '--'.$thr_boundary."--\n\n";

			for ($i=0; $i<count($this->html_images); $i++) {
				$this->multipart.= '--'.$sec_boundary."\n";
				$this->build_html_image($i);
				}

			$this->multipart.= "--".$sec_boundary."--\n\n";
			}
		}

	/***************************************
	** Adds an image to the list of embedded
	** images.
	***************************************/
	function add_html_image($file, $name = '', $c_type='application/octet-stream') {
		$this->html_images[] = array( 'body' => $file,
			'name' => $name,
			'c_type' => $c_type,
			'cid' => md5(uniqid(time())) );
		}

	/***************************************
	** Adds a file to the list of attachments.
	***************************************/
	function add_attachment($file, $name = '', $c_type='application/octet-stream') {
		$this->parts[] = array( 'body' => $file,
			'name' => $name,
			'c_type' => $c_type );
		}

	/***************************************
	** Builds an embedded image part of an
	** html mail.
	***************************************/
	function build_html_image($i) {
		$this->multipart.= 'Content-Type: '.$this->html_images[$i]['c_type'];

		if ($this->html_images[$i]['name'] != '') $this->multipart .= '; name = "'.$this->html_images[$i]['name']."\"\n";
		else $this->multipart .= "\n";

		$this->multipart.= 'Content-ID: <'.$this->html_images[$i]['cid'].">\n";
		$this->multipart.= 'Content-Transfer-Encoding: base64'."\n\n";
		$this->multipart.= chunk_split(base64_encode($this->html_images[$i]['body']))."\n";
		}

	/***************************************
	** Builds a single part of a multipart
	** message.
	***************************************/
	function build_part($i) {
		$message_part = '';
		$message_part.= 'Content-Type: '.$this->parts[$i]['c_type'];

		if ($this->parts[$i]['name'] != '')
			$message_part .= '; name = "'.$this->parts[$i]['name']."\"\n";
		else
			$message_part .= "\n";

		// Determine content encoding.
		if ($this->parts[$i]['c_type'] == 'text/plain') {
			$message_part.= 'Content-Transfer-Encoding: 7bit'."\n\n";
			$message_part.= $this->parts[$i]['body']."\n";
			}
		else {
			$message_part.= 'Content-Transfer-Encoding: base64'."\n";
			$message_part.= 'Content-Disposition: attachment; filename = "'.$this->parts[$i]['name']."\"\n\n";
			$message_part.= chunk_split(base64_encode($this->parts[$i]['body']))."\n";
			}

		return $message_part;
		}

	/***************************************
	** Builds the multipart message from the
	** list ($this->parts).
	***************************************/
	function build_message() {
	$boundary = '=_'.md5(uniqid(time()));
		$this->headers.= "MIME-Version: 1.0\n";
		$this->headers.= "Content-Type: multipart/mixed; boundary = \"".$boundary."\"\n";
		$this->multipart = '';
		$this->multipart.= "This is a MIME encoded message.\n\n";

		if (isset($this->do_html) AND $this->do_html == 1) $this->build_html($boundary);
		if (isset($this->body) AND $this->body != '') $this->parts[] = array('body' => $this->body, 'name' => '', 'c_type' => 'text/plain');

		for ($i=(count($this->parts)-1); $i>=0; $i--) {
			$this->multipart.= '--'.$boundary."\n".$this->build_part($i);
			}

		$this->mime = $this->multipart."--".$boundary."--\n";
		}

	/***************************************
	** Sends the mail.
	***************************************/
	function send($to_name, $to_addr, $from_name, $from_addr, $subject = '', $headers = '') {
		if ($to_name != '') $to = '"'.$to_name.'" <'.$to_addr.'>';
		else $to = $to_addr;

		if($from_name != '') $from = '"'.$from_name.'" <'.$from_addr.'>';
		else $from = $from_addr;

		$this->headers.= 'From: '.$from."\n";
		$this->headers.= $headers;

		return mail($to, $subject, $this->mime, $this->headers);
		}
	} // End of class.
?>