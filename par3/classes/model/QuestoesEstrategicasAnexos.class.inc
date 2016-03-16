<?php
/**
 * Classe de mapeamento da entidade par3.questoesestrategicasanexos
 *
 * @category Class
 * @package  A1
 * @author   EDUARDO DUNICE NETO <eduardo.neto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-10-2015
 * @link     no link
 */



/**
 * Par3_Model_Questoesestrategicasanexos
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 19-10-2015
 * @link     no link
 */
class Par3_Model_QuestoesEstrategicasAnexos extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'par3.questoesestrategicasanexos';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'qeaid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'perid' => array('tabela' => 'questionario.pergunta', 'pk' => 'perid'),
        'qrpid' => array('tabela' => 'questionario.questionarioresposta', 'pk' => 'qrpid'),
        'arqid' => array('tabela' => 'arquivo', 'pk' => 'arqid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qeaid' => null,
        'qrpid' => null,
        'perid' => null,
        'arqid' => null,
    );
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'qeaid' => array(  'Digits'  ),
            'qrpid' => array(  'Digits'  ),
            'perid' => array(  'Digits'  ),
            'arqid' => array(  'Digits'  ),
        );
    }//end getCamposValidacao($dados)


    /**
     * Função carregarResposta
     * - carrega reposta
     *
     */
    public function carregarResposta($qrpid, $perid)
    {
        $sql = "SELECT arqid FROM ".$this->stNomeTabela." WHERE qrpid = $qrpid AND perid = $perid;";

        $arqid = $this->pegaUm($sql);

        $arrArquivo = array();

        if ($arqid != '') {
            $sql = "SELECT * FROM public.arquivo WHERE arqid = $arqid";

            $arrArquivo = $this->pegaLinha($sql);
        }

        return $arrArquivo;
    }//end carregarResposta()


    /**
     * Função verifica
     * - carrega reposta
     *
     */
	public function verifica( $qrpid, $perid )
	{
		$sql = "SELECT qeaid FROM ".$this->stNomeTabela." WHERE qrpid = {$qrpid} AND perid ={$perid}";
		$qeaid = $this->pegaUm( $sql );

		if ($qpaid) {
			$sql = "DELETE FROM ".$this->stNomeTabela." WHERE qeaid = ".$qeaid."; ";
			$this->executar($sql);
		}
		return true;
	}//end verifica()


    /**
     * Função excluirPorArqid
     * - carrega reposta
     *
     */
	public function excluirPorArqid( $arqid )
	{
		if ($arqid) {
			$sql = "DELETE FROM ".$this->stNomeTabela." WHERE arqid = ".$arqid.";";
			$this->executar($sql);
			$this->commit();
		}
		return true;
	}//end excluirPorArqid()


}//end Class
?>