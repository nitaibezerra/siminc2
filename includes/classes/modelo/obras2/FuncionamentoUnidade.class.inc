<?php
	
class FuncionamentoUnidade extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.funcionamentounidade";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "funid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'funid' => null, 
									  	'usucpf' => null, 
									  	'obrid' => null, 
									  	'emsid' => null, 
									  	'empid' => null, 
									  	'supid' => null, 
									  	'funtipofuncionamento' => null, 
									  	'fundtconclusaoobra' => null, 
									    'fundtinauguracao' => null,
									    'funinaugrepmec' => null,
									    'funconveniomob' => null,
									    'funmobadquirido' => null,
									    'funundtfuncionamento' => null,
									    'fununddtprevinauguracao' => null,
									    'fundtprevfuncionamento' => null,
    									'funundfuncionamento' => null,
									    'funqtdcriancacrhparcialefetivo' => null,
									    'funqtdcriancacrhintegralefetivo' => null,
									    'funqtdcriancapreescparefetivo' => null,
									    'funqtdcriancapreescintegfetivo' => null,
									    'funqtdcriancacrhparcialprevisao' => null,
									    'funqtdcriancacrhintprevisao' => null,
									    'funqtdcriancapreescparprevisao' => null,
									    'funqtdcriancapreescintprevisao' => null,
									    'fundtinclusao' => null,
									    'funpercexecutado' => null,
									    'funstatus' => null,
									    'funid_1' => null
									  );

	public function antesSalvar()
	{
		$arrDados = $_POST;
		$arrDados['empid']							 = $arrDados['empid'] ? $arrDados['empid'] : NULL;
		$arrDados['obrid']							 = $arrDados['obrid'] ? $arrDados['obrid'] : NULL;
		$arrDados['usucpf']  						 = $_SESSION['usucpf'];
		$arrDados['funqtdcriancacrhparcialefetivo']  = str_replace(array("."),array(""),$arrDados['funqtdcriancacrhparcialefetivo']);
		$arrDados['funqtdcriancacrhintegralefetivo'] = str_replace(array("."),array(""),$arrDados['funqtdcriancacrhintegralefetivo']);
		$arrDados['funqtdcriancapreescparefetivo']   = str_replace(array("."),array(""),$arrDados['funqtdcriancapreescparefetivo']);
		$arrDados['funqtdcriancapreescintegfetivo']  = str_replace(array("."),array(""),$arrDados['funqtdcriancapreescintegfetivo']);
		$arrDados['funqtdcriancacrhparcialprevisao'] = str_replace(array("."),array(""),$arrDados['funqtdcriancacrhparcialprevisao']);
		$arrDados['funqtdcriancacrhintprevisao']     = str_replace(array("."),array(""),$arrDados['funqtdcriancacrhintprevisao']);
		$arrDados['funqtdcriancapreescparprevisao']  = str_replace(array("."),array(""),$arrDados['funqtdcriancapreescparprevisao']);
		$arrDados['funqtdcriancapreescintprevisao']  = str_replace(array("."),array(""),$arrDados['funqtdcriancapreescintprevisao']);
		$this->popularDadosObjeto($arrDados);
		return true;
	}
	
	public function excluirFuncionamentoUnidade()
	{
		$this->carregarPorId($_POST['funid']);
		$arrDados = $this->getDados();
		$arrDados['funstatus'] = "I";
		$this->popularDadosObjeto($arrDados);
		return $this->salvar(false);
	}
	
	public function listarFuncionamentoUnidade()
	{
		$arrColunas = array(
					"'<img class=\"link\" onclick=\"editarFuncionamentoUnidade(' || funid || ')\" src=\"../imagens/alterar.gif\" /> <img class=\"link\" onclick=\"excluirFuncionamentoUnidade(' || funid || ')\" src=\"../imagens/excluir.gif\" />' as acao",
					"'codigo' as codigo",
					"to_char(fundtinclusao,'DD/MM/YYYY') as fundtinclusao",
					"to_char(funpercexecutado,'999D99') as porcentagem",
					"usunome"
					);
		$arrWhere = array(
							"funstatus = 'A'",
							($_SESSION['obras2']['obrid'] ? "obrid = {$_SESSION['obras2']['obrid']}" : "1=1"),
							($_SESSION['obras2']['empid'] ? "empid = {$_SESSION['obras2']['empid']}" : "1=1"),
							);
		$arrInnerJoin = array("inner" => array("seguranca.usuario usu" => "usu.usucpf = fun.usucpf"));
		$arrCabecalho = array("Ação","Código da Supervisão","Data Inclusão","% de Execução","Responsável");
		$arDados = $this->lista($arrColunas,$arrWhere,$arrInnerJoin,array("alias" => "fun"));
		$arDados = $arDados ? $arDados : array();
		$this->monta_lista($arDados,$arrCabecalho,100,10,"S","center","N","formlista");
	}
}