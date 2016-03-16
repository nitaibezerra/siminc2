<?php

require_once APPRAIZ . 'elabrev/classes/htmlToDocx/phpword/PHPWord.php';
require_once APPRAIZ . 'elabrev/classes/htmlToDocx/simplehtmldom/simple_html_dom.php';
require_once APPRAIZ . 'elabrev/classes/htmlToDocx/htmltodocx_converter/h2d_htmlconverter.php';
require_once APPRAIZ . 'elabrev/classes/htmlToDocx/example_files/styles.inc';

/**
 * Exemplo de uso
 *
 * include_once APPRAIZ . 'path/to/repository/class/HtmlToDocx.class.inc';
 * $html = '<p>Paragrafo para teste</p>';
 *
 * $doc = new HtmlToDocx();
 * $doc->setHtml($html)
 *     ->setNomeDocumento('nome_doc_sem_extensao')
 *     ->getDownload();
 *
 * Fim do exemplo;;
 */

/**
 * Class HtmlToDocx
 * Converte documentos HTML para DocX
 * @documentation https://htmltodocx.codeplex.com/documentation
 */
class HtmlToDocx
{
    protected $phpword_object;

    protected $section;

    protected $html_dom;

    protected $html_dom_array;

    protected $initial_state;

    protected $html;

    protected $h2d_file_uri;

    protected $docName;

    const WRITE_TYPE = 'Word2007';

    public function __construct()
    {
        $this->phpword_object = new PHPWord();
        $this->section = $this->phpword_object->createSection();
        $this->html_dom = new simple_html_dom();
    }

    public function setHtml($html)
    {
        if (!strlen($html)) {
            throw new Exception('HTML precisa ser setado');
        }

        $this->html_dom->load('<html><body>' . $html . '</body></html>');
        $this->html_dom_array = $this->html_dom->find('html', 0)->children();

        $this->setConfig();
        return $this;
    }

    public function setNomeDocumento($nome)
    {
        if (!empty($nome)) {
            $this->docName = $nome;
        } else {
            $this->docName = md5(time());
        }

        return $this;
    }

    private function setConfig()
    {
        $this->initial_state = array(
            // Required parameters:
            'phpword_object' => &$this->phpword_object, // Must be passed by reference.
            'base_root' => 'http://simec-local/ted/', // Required for link elements - change it to your domain.
            'base_path' => '/', // Path from base_root to whatever url your links are relative to.

            // Optional parameters - showing the defaults if you don't set anything:
            'current_style' => array('size' => '11'), // The PHPWord style on the top element - may be inherited by descendent elements.
            'parents' => array(0 => 'body'), // Our parent is body.
            'list_depth' => 0, // This is the current depth of any current list.
            'context' => 'section', // Possible values - section, footer or header.
            'pseudo_list' => TRUE, // NOTE: Word lists not yet supported (TRUE is the only option at present).
            'pseudo_list_indicator_font_name' => 'Wingdings', // Bullet indicator font.
            'pseudo_list_indicator_font_size' => '7', // Bullet indicator size.
            'pseudo_list_indicator_character' => 'l ', // Gives a circle bullet point with wingdings.
            'table_allowed' => TRUE, // Note, if you are adding this html into a PHPWord table you should set this to FALSE: tables cannot be nested in PHPWord.
            'treat_div_as_paragraph' => TRUE, // If set to TRUE, each new div will trigger a new line in the Word document.

            // Optional - no default:
            // This is an array (the "style sheet") - returned by htmltodocx_styles_example() here (in styles.inc) - see this function for an example of how to construct this array.
            'style_sheet' => htmltodocx_styles_example(),
        );

        $this->_buildAndSaveHTML();
    }

    protected function _buildAndSaveHTML()
    {
        // Convert the HTML and put it into the PHPWord object
        htmltodocx_insert_html(
            $this->section,
            $this->html_dom_array[0]->nodes,
            $this->initial_state
        );

        // Clear the HTML dom object:
        $this->html_dom->clear();
        unset($this->html_dom);

        // Save File
        $this->h2d_file_uri = tempnam('', 'htd');
        $objWriter = PHPWord_IOFactory::createWriter($this->phpword_object, self::WRITE_TYPE);
        $objWriter->save($this->h2d_file_uri);
    }

    public function getDownload()
    {
        // Download the file:
        header('Content-Description: File Transfer');
        header('Content-Type: application/octet-stream');
        header('Content-Disposition: attachment; filename='.$this->docName.'.docx');
        header('Content-Transfer-Encoding: binary');
        header('Expires: 0');
        header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
        header('Pragma: public');
        header('Content-Length: ' . filesize($this->h2d_file_uri));
        ob_clean();
        flush();
        readfile($this->h2d_file_uri);
        unlink($this->h2d_file_uri);
    }
}