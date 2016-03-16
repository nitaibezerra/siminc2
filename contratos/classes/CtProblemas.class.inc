<?php
class CtProblemas extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "contratos.ctproblemas";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "prbid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'prbid' => null, 
									  	'ctrid' => null, 
									  	'usucpf' => null, 
									  	'prbtitulo' => null, 
									  	'prbdsc' => null, 
									  	'prbdatainclusao' => null, 
									  	'prbdataocorr' => null, 
									  	'prbstatus' => null, 
									  );
    	
	public function listarProblemas($ctrid) {
		
		$acao = "'<span style=\"white-space:nowrap;\" ><img src=\"../imagens/consultar.gif\" class=\"link\" onclick=\"janela(\'contratos.php?modulo=principal/popupExibirProblema&acao=A&prbid=' || prbid || '\',980,350)\" title=\"Visualizar detalhes\" />'
					as acao";
		
		$sql = "select $acao, to_char(prbdatainclusao,'DD/MM/YYYY HH24:MI:SS') as prbdatainclusao, seg.usunome, to_char(prbdataocorr,'DD/MM/YYYY') as prbdataocorr, prbtitulo
				from $this->stNomeTabela pro
				natural join seguranca.usuario seg
				where ctrid = $ctrid and prbstatus = 'A'
				order by to_char(prbdatainclusao,'DD/MM/YYYY HH24:MI:SS') DESC";
		
		$arrCabecalho = array("Ação","Data de Inclusão","Usuário","Data da Ocorrência","Título do Problema");
		
		$this->monta_lista($sql, $arrCabecalho, 50, 10, "N", "center", "N", '', '50%');
		
	}
	
	public function pesquisaProblema($prbid) {

		$sql = "select to_char(prbdatainclusao,'DD/MM/YYYY') as prbdatainclusao, usu.usunome as usunome, to_char(prbdataocorr,'DD/MM/YYYY') as prbdataocorr, prbtitulo, prbdsc 
				from $this->stNomeTabela pro 
				natural join seguranca.usuario usu 
				where prbid = {$prbid} and prbstatus = 'A'";

		$arrCampos = $this->carregar($sql);
		return $arrCampos;
		
	}
	
	public function salvarProblema() {
		
		if ($this->validarCampos()) {

			$_POST['usucpf'] = $_SESSION['usucpf'];
			$_POST['ctrid'] = $_SESSION['ctrid'];
			$_POST['prbdataocorr'] = formata_data_sql($_POST['prbdataocorr']);
			$horaAtual = date('H:i:s');
			$_POST['prbdatainclusao'] = date('d/m/Y');
			$_POST['prbdatainclusao'] = formata_data_sql($_POST['prbdatainclusao'])." ".$horaAtual;
				
			$this->popularDadosObjeto($_POST);
			$prbid = $this->salvar();
			if ($prbid) {
				$this->commit ();
				$_SESSION ['contratos'] ['problema'] ['alert'] = "Operação realizada com sucesso.";
			}else {
				$_SESSION ['contratos'] ['problema'] ['alert'] = "Não foi possível realizar a operação.";
			}				

			header ( "Location: contratos.php?modulo=principal/cadProblemas&acao=A".($_SESSION['ctrid'] ? "&ctrid=" . $_SESSION['ctrid'] : "") );
			exit ();
				
		}
		
	}
	
	private function validarCampos() {
		$erro = true;
		
		if (! trim ( $_POST ['prbdataocorr'] )) {
			$_SESSION ['contratos'] ['problema'] ['alert'] = 'Favor informar a Data da ocorrência.';
			$erro = false;
		} else if (! trim ( $_POST ['prbtitulo'] )) {
			$_SESSION ['contratos'] ['problema'] ['alert'] = 'Favor informar o Título do problema.';
			$erro = false;
		} else if (! trim ( $_POST ['prbdsc'] )) {
			$_SESSION ['contratos'] ['problema'] ['alert'] = 'Favor informar a Descrição do problema.';
			$erro = false;
		}
	
		if ($erro) {
			return true;
		} else {
			return false;
		}
	}
	
	
}	
?>