<?php
	
class Ligacao extends Modelo{
	
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "callcenter.ligacao";	

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array( "ligid" );

    /**
     * Atributos
     * @var array
     * @access protected
     */    
    protected $arAtributos     = array(
									  	'ligid' => null, 
									  	'temid' => null, 
									  	'tlgid' => null, 
									  	'ligdddtelefone' => null, 
									  	'lignumerotelefone' => null, 
									  	'lignomecontato' => null, 
									  	'ligcargocontato' => null, 
									  	'ligtexto' => null, 
									  	'ligprovidenciamec' => null, 
									  	'ligprovidenciacontato' => null, 
									  	'usucpf' => null, 
									  	'ligdata' => null
									  );
									  
	public function carregaDadosLigacao($ligid){
		global $db;
		
		$sql = "SELECT lig.ligid, tem.temdescricao, tli.tlgdescricao, lig.ligdddtelefone, lig.lignumerotelefone, lig.lignomecontato,
				  lig.ligcargocontato, lig.ligtexto, lig.ligprovidenciamec, lig.ligprovidenciacontato, usu.usunome,
				  lig.ligdata, lig.ligdataprovidenciamec, lig.ligdataprovidenciacontato, lig.usucpf, lig.temid, lig.tlgid
				FROM 
				  callcenter.ligacao lig
				  inner join seguranca.usuario usu on usu.usucpf = lig.usucpf
				  inner join callcenter.tipoligacao tli on tli.tlgid = lig.tlgid
				  inner join callcenter.tema tem on tem.temid = lig.temid WHERE ligid = ".$ligid;
		$arLigacao = $db->pegaLinha( $sql );
		$arLigacao = $arLigacao ? $arLigacao : array();
		
		return $arLigacao;
	}
	public function insereLigacao($post){
		global $db;
		extract($post);
		
		$ligdataprovidenciamec = ($ligdataprovidenciamec ? "'".formata_data_sql($ligdataprovidenciamec)."'" : 'null');
		$ligdataprovidenciacontato = ($ligdataprovidenciacontato ? "'".formata_data_sql($ligdataprovidenciacontato)."'" : 'null');
	
		$sql = "INSERT INTO callcenter.ligacao(temid, tlgid, ligdddtelefone, lignumerotelefone, lignomecontato, ligcargocontato,
	  				ligtexto, ligprovidenciamec, ligprovidenciacontato, usucpf, ligdata, ligdataprovidenciamec, ligdataprovidenciacontato) 
				VALUES ('{$_SESSION['callcenter']['temid']}', '{$tlgid}', '{$ligdddtelefone}', '{$lignumerotelefone}', '{$lignomecontato}', '{$ligcargocontato}',
					'{$ligtexto}', '{$ligprovidenciamec}', '{$ligprovidenciacontato}', '{$_SESSION['usucpf']}', 'now()', $ligdataprovidenciamec, $ligdataprovidenciacontato) RETURNING ligid";
		
		$ligid = $db->pegaUm( $sql );
		return $ligid;
	}
	public function alteraLigacao($post){
		global $db;
		extract($post);
		
		$ligdataprovidenciamec = ($ligdataprovidenciamec ? "'".formata_data_sql($ligdataprovidenciamec)."'" : 'null');
		$ligdataprovidenciacontato = ($ligdataprovidenciacontato ? "'".formata_data_sql($ligdataprovidenciacontato)."'" : 'null');
		
		$sql = "UPDATE callcenter.ligacao SET
					temid = '{$_SESSION['callcenter']['temid']}',
				  	tlgid = '{$tlgid}',
				  	ligdddtelefone = '{$ligdddtelefone}',
				  	lignumerotelefone = '{$lignumerotelefone}',
				  	lignomecontato = '{$lignomecontato}',
				  	ligcargocontato = '{$ligcargocontato}',
				  	ligtexto = '{$ligtexto}',
				  	ligprovidenciamec = '{$ligprovidenciamec}',
				  	ligprovidenciacontato = '{$ligprovidenciacontato}',
				  	usucpf = '{$_SESSION['usucpf']}',
				  	ligdata = now(),
				  	ligdataprovidenciamec = $ligdataprovidenciamec,
				  	ligdataprovidenciacontato = $ligdataprovidenciacontato
				WHERE 
				  ligid = ".$_REQUEST['ligid'];
		$db->executar($sql);
		return $db->commit();
	}
	function montaListaLigacao( $post ){
		global $db;
		monta_titulo('', '<b>Histórico de Ligação</b>');
		extract($post);
		
		$arJoin = array();
		$arFiltro = array();
		switch ($_SESSION['callcenter']['temid']) {
			case 1:
				array_push( $arFiltro, 'and ate.proid = '.$proid );
				array_push( $arJoin, 'inner join par.atendimento ate on ate.ligid = lig.ligid' );
			break;
			
			default:
				;
			break;
		}
		
		//<img src=\"/imagens/alterar.gif\" style=cursor:pointer; title=\"Recebida\" onclick=\"alterarLigacao('||lig.ligid||');\">
		$sql = "SELECT
					'<center>
					 <img src=\"/imagens/consultar.gif\" style=cursor:pointer; title=\"Recebida\" onclick=\"mostraLigacao('||lig.ligid||');\">
					 </center>' as acoes,
					tpl.tlgdescricao, '('||lig.ligdddtelefone ||') '|| lig.lignumerotelefone, lig.lignomecontato, 
					lig.ligcargocontato, usu.usunome, to_char(ligdata, 'DD/MM/YYYY')
				FROM callcenter.ligacao lig
					inner join seguranca.usuario usu on usu.usucpf = lig.usucpf
				    inner join callcenter.tipoligacao tpl on tpl.tlgid = lig.tlgid and tpl.tlgstatus = 'A'
				    ".implode( '', $arJoin )."
				WHERE
					temid = ".$_SESSION['callcenter']['temid']."
					and tpl.tlgid = {$tlgid} ".
					implode( ' and ', $arFiltro );
		
		$cabecalho = array("Ações", "Tipo de Ligação", "Telefone", "Contato", "Cargo", "Usuário", "Data");
		return $db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
	}
}