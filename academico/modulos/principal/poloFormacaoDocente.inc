<?php
if($_REQUEST['requisicao'] == "removepolo"){
	$entid = $_GET['entid'];
	$sql = "UPDATE entidade.funcaoentidade set fuestatus = 'I' where entid = $entid and funid = ".FUNCAO_ENTIDADE_POLO_FORMACAO_DOCENTE;
	$db->executar($sql);
	$db->commit();
	echo "<script type=\"text/javascript\">
        		alert('Polo excluído com sucesso!');
        		window.location='academico.php?modulo=principal/poloFormacaoDocente&acao=A';
        	  </script>";
        exit;	
}

if ($_REQUEST['opt']) {
	if ($_REQUEST['opt'] == 'salvarRegistro') {
		require_once APPRAIZ . "includes/classes/entidades.class.inc";
		$entidade = new Entidades();
		$entidade->carregarEntidade($_REQUEST);
		$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
		$entidade->salvar();
		$entid = $entidade->getEntId();
        echo "<script type=\"text/javascript\">
        		alert('Polo gravado com sucesso!');
        		window.opener.location.href = window.opener.location;
        		window.location='academico.php?modulo=principal/poloFormacaoDocente&acao=A&requisicao=addpolo&entid=".$entid."';
        	  </script>";
        exit;
    }
}
if($_REQUEST['requisicao'] == "addpolo"){
	require_once APPRAIZ . "includes/classes/entidades.class.inc";
	?>
	<script type="text/javascript" src="../includes/entidades.js"></script>
	<script type="text/javascript" src="../includes/prototype.js"></script>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	
	<?php
	$entid = $_GET['entid'];
	$entidade = new Entidades();
	if($entid){
		$entidade->carregarPorEntid($entid);
	}
	echo $entidade->formEntidade("academico.php?modulo=principal/poloFormacaoDocente&acao=A&opt=salvarRegistro",
							 array("funid" => FUNCAO_ENTIDADE_POLO_FORMACAO_DOCENTE, "entidassociado" => $_SESSION['academico']['entid']),
							 array("enderecos"=>array(1),"gravar"=>true),"J",'',''
							 );
	exit;
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( "Polos de Formação Docente", "");

if(!$_SESSION['academico']['entid']){
	echo "<script>alert('Institução não encontrada!');history.back(-1);</script>";
}

$sql = "select 
			'<img src=\"../imagens/alterar.gif\" style=\"cursor:pointer\" onclick=\"alteraPolo(\'' || ent.entid || '\')\"  /> <img style=\"cursor:pointer\" src=\"../imagens/excluir.gif\" onclick=\"excluirPolo(\'' || ent.entid || '\')\"  />' as acao,
			ent.entnome
		from
			entidade.entidade ent
		inner join
			entidade.funcaoentidade func ON ent.entid = func.entid
		inner join
			entidade.funentassoc fue ON fue.fueid = func.fueid
		where
			func.funid = ".FUNCAO_ENTIDADE_POLO_FORMACAO_DOCENTE."
		and
			fue.entid = {$_SESSION['academico']['entid']}
		and
			ent.entstatus = 'A'
		and
			fuestatus = 'A'";
$arrCabecalho = array("Ação","Polo");
$db->monta_lista($sql,$arrCabecalho,100,10,"N","center","N");
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td><input type="button" name="btn_cadastrar" value="Adicionar Polo" onclick="addPolo()" /></td>
	</tr>
</table>
<script>
	function addPolo()
	{
		var janela = window.open("academico.php?modulo=principal/poloFormacaoDocente&acao=A&requisicao=addpolo", "polo", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=500");
		janela.focus();
	}
	
	function alteraPolo(entid)
	{
		var janela = window.open("academico.php?modulo=principal/poloFormacaoDocente&acao=A&requisicao=addpolo&entid=" + entid, "polo", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=500");
		janela.focus();
	}
	
	function excluirPolo(entid)
	{
		if(confirm("Deseja realmente remover este Polo?")){
			window.location.href="academico.php?modulo=principal/poloFormacaoDocente&acao=A&requisicao=removepolo&entid=" + entid;
		}
	}
	
</script>