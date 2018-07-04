<?php
/**
 * Classe de mapeamento da entidade public.arquivo_modulo
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafael.carneiro@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 31-05-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Public_Model_ArquivoModulo
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_ArquivoModulo extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.arquivo_modulo';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'arqmdid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'arqmdid' => null,
        'arqid' => null
    );
    
    public function recuperaArquivosPorModulo(){
        $sql = "select a.*,
                       am.*,
                       u.usunome,
                       concat(to_char(a.arqdata,'DD/MM/YYYY'),' ', a.arqhora) as data
                  from ".$this->stNomeTabela." am 
                 inner join public.arquivo a
                    on am.arqid = a.arqid
                 inner join seguranca.usuario u
                    on a.usucpf = u.usucpf
                 where a.sisid = ".$_SESSION['sisid']."
                   and a.arqstatus = 'A'";
        return $this->carregar($sql);
    }
    
    public function excluirArquivo(){
        $sql = "update public.arquivo set arqstatus = 'I' where arqid = ".$this->arqid;
        $this->executar($sql);
    }
}//end Class
?>