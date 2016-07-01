<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
    
    <title>Mapa de Obras</title>
    <?
		// Carrega os dados das obras
   		$preid = $_REQUEST["preid"];
		
	if ( empty( $preid ) ){
		print "<script>"
			. "		alert('A sessão da obra escolhida expirou!');"
			. "		window.location='/par/par.php?modulo=principal/listaMunicipios&acao=A';"
			. "</script>";
	}
		
	// Parâmetros para o Sildeshow selecionar as imagens da obra.
	$_SESSION['imgparams'] = array("filtro" => "preid = ".$preid." AND arqstatus = 'A'", "tabela" => "obras.preobrafotos");
		
	
	$sql = "SELECT 
				pre.preid as id_preobra,
				ent.entnome as entidade,
				preid || ' - ' || predescricao as descricao,
				mun.estuf || ' - ' || mun.mundescricao as municipio, 
				prelogradouro || ' , ' || precomplemento || ' / ' || prebairro as endereço, 
				tpo.ptodescricao as tipo, 
				ed.esddsc as situacao,
				substring(pre.precep,1,5) || '-' || substring(pre.precep,6,8) as cep, 
		        to_char(pre.predtinclusao, 'DD/MM/YYYY') as inclusao,
		        preano as ano
			        
			FROM 
				obras.preobra pre
			LEFT JOIN
				entidade.entidade       ent ON ent.entcodent = pre.entcodent
			LEFT JOIN
				territorios.municipio   mun ON mun.muncod = pre.muncod
			LEFT JOIN
				obras.pretipoobra       tpo ON tpo.ptoid = pre.ptoid
			LEFT JOIN
				workflow.documento      doc ON doc.docid = pre.docid
			LEFT JOIN
				workflow.estadodocumento ed ON ed.esdid = doc.esdid 
			WHERE
				preid = {$preid}";
		//dbg ($sql,1);
		
	$dados = $db->carregar($sql);			   
	
	if($dados){
		for($i=0;$i < count($dados);$i++){
			$id_preobra=$dados[$i]["id_preobra"];
			// Fotos
			$imagens="";
			$sql = "SELECT 
						arq.arqnome, 
						arq.arqid, 
						arq.arqdescricao, 
						to_char(arq.arqdata, 'dd/mm/yyyy') as arqdata 
					FROM public.arquivo arq
					INNER JOIN obras.preobrafotos      pref ON pref.arqid = arq.arqid 
					INNER JOIN obras.preobra 	    pre ON pre.preid  = pref.preid
					WHERE 
						pre.preid = {$id_preobra} AND
						arqstatus = 'A' AND
						(arqtipo   = 'image/jpeg' OR
						   arqtipo = 'image/gif'  OR
						   arqtipo = 'image/png') 
					ORDER BY 
						arq.arqid DESC 
					LIMIT 3";
			
			$fotos = ($db->carregar($sql));
			if ($fotos){
				for($i=0;$i < count($fotos);$i++){
					$imagens_fotos.="<img src='../slideshow/slideshow/verimagem.php?newwidth=80&amp;newheight=60&amp;arqid=".$fotos[$i]["arqid"]."&_sisarquivo=obras' hspace='0' vspace='2' style='width:80px; height:60px;' onClick='javascript: abrir_fotos(".$fotos[$i]["arqid"].")'> <br> ".$fotos[$i]["arqdata"]." <br>".$fotos[$i]["arqdescricao"]." <br>";										
				}
			}
		}			
	}
?>
<style>
	body {
	BORDER-BOTTOM: 0px; BORDER-LEFT: 0px; BORDER-RIGHT: 0px; BORDER-TOP: 0px}
</style>
<script type="text/javascript">
var preid  = <?=$dados[0]["id_preobra"] ?>;
var preano = <?=$dados[0]["ano"] ?>;

function abrir(){
	var janela = window.open( 'http://simec-local/par/par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+preid+'&ano='+preano, 'resultadoObrasGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	janela.focus();
}
function abrir_fotos( foto ){
	window.open("../slideshow/slideshow/index.php?pagina=&_sisarquivo=obras&amp;arqid="+foto,"imagem","width=850,height=600,resizable=yes");
}

</script>
</head>
<body>
	<table class=tabela width=300>
		<tr>
			<td valign="top">
				<b><font size=1 face=arial><?= $dados[0]["entidade"] ?></font></b>
				<br><b><font color=blue size=1 face=arial><?=$dados[0]["descricao"]; ?></font></b>
				<br><b><font color=blue size=1 face=arial><?=$dados[0]["municipio"]; ?></font></b>
				<br><font size=1 face=arial color=#660000><?=$dados[0]["endereço"]; ?></font>
				<br><font size=1 face=arial color=#660000><?=$dados[0]["cep"]; ?></font>
				<br><font size=1 face=arial color=#660000> Situação:<?=$dados[0]["situacao"]; ?></font>
				<br><font size=1 face=arial color=#660000> Tipo:<?=$dados[0]["tipo"]; ?></font>
				<br><font size=1 face=arial color=#660000> Data de Inclusão: <?= $dados[0]["inclusao"]; ?></font>
				<br><font size=1 face=arial color=#660000> Ano previsto: <?= $dados[0]["ano"]; ?></font>
			</td>
			<td colspan=2 valign="top">
				<font size=1 face=arial><?=$imagens_fotos; ?></font>
			</td>
		</tr>
	</table>	
</body>
</html>
