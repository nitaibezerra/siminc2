<?

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/escolaterra.js"></script>';

echo "<br>";

monta_titulo( "Central de Acompanhamento", "Informações sobre o andamento do projeto");

$menu[] = array("id" => 1, "descricao" => 'Etapa Cadastramento', "link" => '/escolaterra/escolaterra.php?modulo=centralacompanhamento&acao=A&aba=cadastramento');
$menu[] = array("id" => 2, "descricao" => 'Etapa Execução', "link" => '/escolaterra/escolaterra.php?modulo=centralacompanhamento&acao=A&aba=execucao');
echo "<br>";

if(!$_REQUEST['aba']) $_REQUEST['aba']='cadastramento';

echo montarAbasArray($menu, '/escolaterra/escolaterra.php?modulo=centralacompanhamento&acao=A&aba='.$_REQUEST['aba']);

if($_REQUEST['aba']=='cadastramento') :
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" valign="top" width="50%">
	<p align="center"><b>Situação do cadastramento dos Tutores</b></p>
	<?

	$sql = "SELECT e.esddsc, count(*) as numero FROM escolaterra.turmas tu 
			INNER JOIN escolaterra.identificacaousuario i ON i.iusid = tu.iusid AND i.iusstatus='A' 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid AND t.pflcod=".PFL_COORDENADORESTADUAL." 
			INNER JOIN workflow.documento d ON d.docid = tu.docid 
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			GROUP BY e.esddsc";
	
	$cabecalho = array("Situação","Quantidade");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%');
	
	?>
	
	<p align="center"><b>Situação do cadastramento dos Professores</b></p>
	<?

	$sql = "SELECT e.esddsc, count(*) as numero FROM escolaterra.turmas tu 
			INNER JOIN escolaterra.identificacaousuario i ON i.iusid = tu.iusid AND i.iusstatus='A' 
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid AND t.pflcod=".PFL_TUTOR." 
			INNER JOIN workflow.documento d ON d.docid = tu.docid 
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			GROUP BY e.esddsc";
	
	$cabecalho = array("Situação","Quantidade");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%');
	
	?>
	</td>
	
	<td class="SubTituloCentro" valign="top" width="50%">
	<p align="center"><b>Quantitativos dos perfis cadastrados</b></p>
	<?

	$sql = "SELECT p.pfldsc, count(*) as numero FROM escolaterra.identificacaousuario i  
			INNER JOIN escolaterra.tipoperfil t ON t.iusid = i.iusid 
			INNER JOIN seguranca.perfil p ON p.pflcod = t.pflcod 
			WHERE i.iusstatus='A' 
			GROUP BY p.pfldsc";
	
	$cabecalho = array("Perfil","Quantidade");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%');
	
	?>
	
	</td>
</tr>
</table>
<? elseif($_REQUEST['aba']=='execucao') : ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro" valign="top" width="50%">
	<p align="center"><b>Situação dos relatórios - Tutores</b></p>
	<?

	$sql = "SELECT e.esddsc, count(*) as n FROM escolaterra.relatorioacompanhamento r 
			INNER JOIN workflow.documento d ON d.docid = r.docid 
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			GROUP BY e.esddsc";
	
	$cabecalho = array("Perfil","Quantidade");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%');
	
	?>

	</td>
	
	<td class="SubTituloCentro" valign="top" width="50%">
	<p align="center"><b>Situação dos relatórios - Coordenadores Estaduais</b></p>
	<?

	$sql = "SELECT e.esddsc, count(*) as n FROM escolaterra.relatorioacompanhamentocoordenadorestadual r 
			INNER JOIN workflow.documento d ON d.docid = r.docid 
			INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
			GROUP BY e.esddsc";
	
	$cabecalho = array("Perfil","Quantidade");
	$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','100%');
	
	?>
	
	</td>
</tr>
</table>
<? endif; ?>