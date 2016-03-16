
<?php
/**
 * @version $Id: Exception.php 24594 2012-01-05 21:27:01Z matthew $
 */

/**
 * @category   Zend
 * @package    Zend_Db
 * @copyright  Copyright (c) 2005-2012 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 */
class Simec_Db_Exception extends Zend_Db_Exception
{
    protected $detalhe = array();

    /**
     *
     * @param string $message Mensagem de descrição da exception.
     */
    public function __construct($message = null, $detalhe = null)
    {
        if (!empty($detalhe)) {
            $this->setDetalhe($detalhe);
            $message = $message . $this->detalhe;
        }
        
        parent::__construct($message);
    }

    /**
     * Detalhamento da exception.
     *
     * @param string|array $detalhe Detalhamento
     * @return $this
     */
    public function setDetalhe($detalhes)
    {
        if (is_array($detalhes)) {
        	$mensagem = "";
        	foreach ($detalhes as $id => $detalhe) {
        		$mensagem.= "\r\n" . $detalhe;        		
        	}
        	$this->detalhe = $mensagem;
        } else {
            $this->detalhe[] = $detalhes;
        }
        return $this;
    }

    /**
     * @return array
     */
    public function getDetalhe()
    {
        return $this->detalhe;
    }
}
