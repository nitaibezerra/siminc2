<?php

/**
 * Class HtmlToPdf
 */
class HtmlToPdf
{
    /**
     * endpoint para o webservice para geração de PDF
     */
    const END_POINT = 'http://ws.mec.gov.br/ws-server/htmlParaPdf';

    /**
     * Metodo de envio
     */
    const METHOD = 'POST';

    /**
     * Recebe o conteudo HTML a ser convertido para PDF
     */
    protected $_htmlContent;

    /**
     * Define um titulo para o documento a ser criado
     */
    protected $_title;

    /**
     * @param $html
     */
    public function __construct($html)
    {
        if (empty($html)) {
            throw new Exception('Construct expected html string content');
        }
        $this->_htmlContent = $html;
        return $this;
    }

    /**
     * Seta um titulo para o documento
     * @param $title
     */
    public function setTitle($title)
    {
        $this->_title = $title;
        return $this;
    }

    /**
     * Recupera o titulo
     * @return mixed
     */
    public function getTitle()
    {
        return $this->_title;
    }

    /**
     * Limpa os parametros usados para a criação em lote
     * @return $this
     */
    public function clearParams()
    {
        unset($this->_title, $this->_htmlContent);
        return $this;
    }

    /**
     * Faz a processamento para geração do PDF
     * @return void(0)
     */
    public function getPDF()
    {
        $content = http_build_query(array(
            'conteudoHtml' => utf8_encode($this->_htmlContent)
        ));

        $paramContext = array(
            'http' => array(
                'method' => self::METHOD,
                'content' => $content
            )
        );

        $context = stream_context_create($paramContext);
        $contents = file_get_contents(self::END_POINT, null, $context);

        header('Content-Type: application/pdf');
        header("Content-Disposition: attachment; filename={$this->getTitle()}");
        echo $contents;
        exit();
    }
    
    /**
     * Faz a processamento para geração do PDF
     * @return void(0)
     */
    public function getContent()
    {
    	$content = http_build_query(array(
    		'conteudoHtml' => utf8_encode($this->_htmlContent)
    	));
    
    	$paramContext = array(
    		'http' => array(
    			'method' => self::METHOD,
    			'content' => $content
    		)
    	);
    
    	$context = stream_context_create($paramContext);
    	$contents = file_get_contents(self::END_POINT, null, $context);
    
    	return $contents;
    }
}