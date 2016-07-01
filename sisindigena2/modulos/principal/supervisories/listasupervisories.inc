<?
include "_funcoes_supervisories.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisindigena2.js"></script>';

echo "<br>";

monta_titulo( "Lista - Supervisor IES", "Lista de Supervisores IES");

?>
<script>
function gerenciarSupervisorIES(iusd, picid) {
	window.open('sisindigena2.php?modulo=principal/supervisories/gerenciarsupervisories&acao=A&iusd='+iusd+'&picid='+picid,'SupervisorIES','scrollbars=no,height=600,width=800,status=no,toolbar=no,menubar=no,location=no');
}
</script>
<?

$sql = "SELECT 
			'<img src=\"../imagens/alterar.gif\" border=\"0\" style=\"cursor:pointer;\" onclick=\"window.location=\'sisindigena2.php?modulo=principal/supervisories/supervisories&acao=A&iusd='||foo.iusd||'&requisicao=carregarSupervisorIES&direcionar=true\';\">  <img src=\"../imagens/usuario.gif\" border=\"0\" onclick=\"gerenciarSupervisorIES(\''||foo.iusd||'\',\''||foo.picid||'\');\" style=\"cursor:pointer;\">' as acao,
			CASE WHEN foo.status='A' THEN '<img src=\"../imagens/p_verde.gif\" border=\"0\" align=\"absmiddle\">' ELSE '<img src=\"../imagens/p_vermelho.gif\" border=\"0\" align=\"absmiddle\">' END ||' '|| 
			CASE WHEN foo.status IS NULL THEN 'Não Cadastrado'
		   		 WHEN foo.status='A'		THEN 'Ativo'
				 WHEN foo.status='B'		THEN 'Bloqueado' 
				 WHEN foo.status='P'		THEN 'Pendente' END as situacao,
			foo.iuscpf,
			foo.iusnome,
			foo.iusemailprincipal,
			CASE WHEN foo.iustermocompromisso=true THEN '<span style=color:blue;>Sim</font>' ELSE '<span style=color:red;>Não</font>' END as termo,
			foo.esddsc,
			foo.uninome
		FROM (
		SELECT
		i.iusd, 
		i.uncid, 
		i.iuscpf,
		i.iusnome,
		i.iusemailprincipal,
		i.iustermocompromisso,
		un.picid,
		COALESCE(e.esddsc,'Não iniciado') as esddsc,
		(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=i.iuscpf AND sisid=".SIS_INDIGENA.") as status,
		(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=i.iuscpf AND pflcod=".PFL_SUPERVISORIES.") as perfil,
		uu.unisigla ||' - '|| uu.uninome as uninome
		FROM sisindigena2.identificacaousuario i 
		INNER JOIN sisindigena2.tipoperfil t ON t.iusd = i.iusd 
		LEFT JOIN sisindigena2.turmas tu ON tu.iusd = i.iusd 
		LEFT JOIN sisindigena2.nucleouniversidade un ON un.picid = i.picid 
		LEFT JOIN sisindigena2.universidade uu ON uu.uniid = un.uniid 
		LEFT JOIN workflow.documento d ON d.docid = tu.docid 
		LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
		WHERE i.iusstatus='A' AND t.pflcod='".PFL_SUPERVISORIES."') foo 
		ORDER BY foo.iusnome";

$cabecalho = array("&nbsp;","Situação","CPF","Nome","E-mail","Termo aceito?","Cadastramento","Núcleo");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);
?>