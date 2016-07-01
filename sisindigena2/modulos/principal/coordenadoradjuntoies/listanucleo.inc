<?
include "_funcoes_coordenadoradjuntoies.php";


if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}


include  APPRAIZ."includes/cabecalho.inc";
echo '<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>';
echo '<script language="javascript" type="text/javascript" src="./js/sisindigena2.js"></script>';

echo "<br>";

monta_titulo( "Lista - Coordenador Adjunto IES", "Lista de Coordenadores Adjunto IES");


$sql = "SELECT foo.acao1,
			   foo.uninome,
			   foo.situacao,
			   foo.estuf,
			   foo.mundescricao,
			   foo.coordenadoradjunto,
			   foo.uninomesede,
			   foo.termo
		FROM(
		SELECT '<center><img src=\"../imagens/alterar.gif\" border=0 style=\"cursor:pointer;\" onclick=\"window.location=\'sisindigena2.php?modulo=principal/coordenadoradjuntoies/coordenadoradjuntoies&acao=A&picid='||u.picid||'&requisicao=carregarCoordenadorAdjuntoIES&direcionar=true\';\"> <img src=\"../imagens/usuario.gif\" border=\"0\" onclick=\"gerenciarCoordenadorAdjunto(\''||u.picid||'\');\" style=\"cursor:pointer;\"></center>' as acao1,
				su.unisigla||' - '||su.uninome as uninome,
				COALESCE((SELECT iusnome FROM sisindigena2.identificacaousuario i INNER JOIN sisindigena2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=u.picid AND t.pflcod=".PFL_COORDENADORADJUNTOIES."),'Coordenador Adjunto não cadastrado') as coordenadoradjunto,
				COALESCE((SELECT CASE WHEN iustermocompromisso=true THEN 'Sim' ELSE 'Não' END FROM sisindigena2.identificacaousuario i INNER JOIN sisindigena2.tipoperfil t ON i.iusd=t.iusd WHERE i.picid=u.picid AND t.pflcod=".PFL_COORDENADORADJUNTOIES."),'Coordenador Adjunto não cadastrado') as termo,
				su.uniuf as estuf,
				m.muncod,
				m.mundescricao,
				su2.unisigla||' - '||su2.uninome as uninomesede,
				e.esddsc as situacao
		FROM sisindigena2.nucleouniversidade u 
		LEFT JOIN workflow.documento d ON d.docid = u.docid 
		LEFT JOIN workflow.estadodocumento e ON e.esdid = d.esdid  
		INNER JOIN sisindigena2.universidade su ON su.uniid = u.uniid 
		INNER JOIN sisindigena2.universidadecadastro unc ON unc.uncid = u.uncid 
		INNER JOIN sisindigena2.universidade su2 ON su2.uniid = unc.uniid
		LEFT JOIN territorios.municipio m ON m.muncod = su.muncod 
		WHERE u.picstatus='A'
		) foo
		ORDER BY foo.uninomesede,foo.uninome";
	
$cabecalho = array("&nbsp;","Universidade","Situação","UF","Município","Coordenador Núcleo","Universidade Sede","Termo");
$db->monta_lista($sql,$cabecalho,50,10,'N','center',$par2);

?>