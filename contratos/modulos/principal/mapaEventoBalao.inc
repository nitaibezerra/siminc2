<html>
  <head>
    <meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>
    
    <title>Mapa de Eventos</title>
    <?
		// Carrega os dados das Eventos
   		$IdEvento=$_REQUEST["obrid"];
		
    	// Parâmetros para o Sildeshow selecionar as imagens da Evento.
		$_SESSION['imgparams'] = array("filtro" => "cnt.obrid=".$IdEvento." AND aqostatus = 'A'", "tabela" => "Eventos.arquivosEvento");
		$sql="
			SELECT 
				oi.obrid as id_Evento,    
				UPPER( oi.obrdesc ) as nome_Evento,
				sto.stodesc as situacao,
	            obrcomposicao, 
				REPLACE(oi.obrpercexec||'','.',',') || ' %'  as percentual,
				oi.obrcustocontrato as valor, 
                   et.entnome as unidade
			FROM 
				Eventos.Eventoinfraestrutura oi 
			INNER JOIN 
				entidade.entidade et ON oi.entidunidade = et.entid 
			LEFT JOIN 
				Eventos.situacaoEvento sto ON oi.stoid = sto.stoid
			INNER JOIN 
				Eventos.orgao org ON oi.orgid = org.orgid
			 WHERE  oi.obrid = $IdEvento
			GROUP BY 
                oi.obrid,oi.obrdesc, sto.stodesc, obrcomposicao,oi.obrpercexec, oi.obrcustocontrato,  et.entnome";
			//dbg ($sql,1);
			$dados = $db->carregar($sql);			   
	
			if($dados){
				for($i=0;$i < count($dados);$i++){
					$id_Evento=$dados[$i]["id_Evento"];
					// Fotos
					$imagens="";
						$sql = "SELECT arqnome, arq.arqid, arq.arqdescricao, to_char(arq.arqdata, 'dd/mm/yyyy') as arqdata 
									FROM public.arquivo arq
									INNER JOIN Eventos.arquivosEvento oar ON arq.arqid = oar.arqid
									INNER JOIN Eventos.Eventoinfraestrutura obr ON obr.obrid = oar.obrid 
									INNER JOIN seguranca.usuario seg ON seg.usucpf = oar.usucpf 
								WHERE obr.obrid = $IdEvento AND
									  aqostatus = 'A' AND
									  (arqtipo = 'image/jpeg' OR 
									   arqtipo = 'image/gif' OR 
									   arqtipo = 'image/png') 
								ORDER BY arq.arqid DESC LIMIT 3";
					$fotos = ($db->carregar($sql));
					if ($fotos){
						for($i=0;$i < count($fotos);$i++){
							$imagens_fotos.="<img src='../../slideshow/slideshow/verimagem.php?newwidth=80&newheight=60&arqid=".$fotos[$i]["arqid"]."' hspace='0' vspace='2' style='width:80px; height:60px;' onClick='abrir_fotos(".$fotos[$i]["arqid"].")'> <br> ".$fotos[$i]["arqdata"]." <br>".$fotos[$i]["arqdescricao"]." <br>";										
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
function abrir(){
	janela=parent.window.location.href='/Eventos/Eventos.php?modulo=principal/cadastro&acao=A&obrid=<?= $IdEvento;?>';
	janela.focus();
}
function abrir_fotos(foto){
	window.open("../slideshow/slideshow/index.php?pagina=&amp;arqid="+foto,"imagem","width=850,height=600,resizable=yes")
}

</script>
  </head>
  <body>
	<table class=tabela width=300>
	<tr>
		<td valign="top">
		<b><font size=1 face=arial><?= $dados[0]["unidade"] ?></font></b>
		<br><b><font color=blue size=1 face=arial><a href=# onClick="javascript: abrir();"><?=$dados[0]["nome_Evento"]; ?></a></font></b>
		<br><font size=1 face=arial color=#660000><?= "Situação: ".$dados[0]["situacao"]; ?></font>
		<br><font size=1 face=arial color=#FFCC33> Executado: <?= $dados[0]["percentual"] ?></font>
		<br><font size=1 face=arial color=#3399FF> Valor: R$ <?= number_format($dados[0]["valor"] ,2,',','.');?></font>
		<br><br><font size=1 face=arial><?=$dados[0]["obrcomposicao"]; ?></font></td>
		<td colspan=2 valign="top"><font size=1 face=arial><?=$imagens_fotos; ?></font></td>
		</tr>
	</table>	
  </body>
</html>

