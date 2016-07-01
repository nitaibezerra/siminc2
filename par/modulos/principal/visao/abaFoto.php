<?php 
	$caminho_atual = $_SERVER['REQUEST_URI'];
	$subid = $_REQUEST['icoid'] ? $_REQUEST['icoid'] : 1; 
?>
<script>
function excluirFoto(url, arqid, fotid){
	if(confirm("Deseja realmente excluir esta foto ?")){
		window.location = url+'&fotid='+fotid+'&arqid='+arqid+'&itemid='+<?php echo $_REQUEST['icoid']?>;
	}
}
</script>
<?php 
if($_GET['requisicao'] == 'excluir'){
	
	$sql = "DELETE FROM obras.preobrafotos WHERE pofid = {$_GET['fotid']}";
	if($db->executar($sql)){
		$sql = "DELETE FROM public.arquivo WHERE arqid = {$_GET['arqid']}";
		$db->executar($sql);
		$db->commit();
	}				
	echo '<script>
			alert("Foto excluída com sucesso!");
			document.location.href = \'par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=foto&icoid='.$_GET['icoid'].'\';
		  </script>';
	exit;
}
?>				
<?php echo carregaAbasItensComposicao("par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=foto&icoid=".$_REQUEST['icoid'], $_REQUEST['icoid'],$descricaoItem); ?>
<?php 
## UPLOAD DE ARQUIVO
$campos	= array("preid"	=> $_SESSION['par']['preid'],
				"pofdescricao" => "'".$_POST['fotdescricao']."'");
	
$file = new FilesSimec("preobrafotos", $campos, 'obras');
if($_FILES["Arquivo"]){	
	$arquivoSalvo = $file->setUpload($_POST['fotdescricao']);
	if($arquivoSalvo){
		echo '<script type="text/javascript"> 
					alert("Foto gravada com sucesso.");
					document.location.href = \''.$caminho_atual.'\';
			  </script>';
	}
}
?>
<?php monta_titulo( 'Cadastro de fotos', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  ); ?>
<form action="" method="post" enctype="multipart/form-data">			
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr>
			<td class="SubTituloDireita">Descrição da foto:</td>
			<td>						
				<?php echo campo_texto('fotdescricao', 'S', 'S', '', 30, 255, '', '') ?>						
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Enviar foto:</td>
			<td>
				<input type="file" name="Arquivo">							
			</td>
		</tr>				
		<tr>
			<td colspan="2" class="SubTituloEsquerda"><input type="submit" value="Salvar"></td>
		</tr>
		<tr>
			<td colspan="2" class="SubTituloCentro">
				<link rel="stylesheet" type="text/css" href="includes/superTitle.css"/>
				<script type="text/javascript" src="includes/remedial.js"></script>
				<script type="text/javascript" src="includes/superTitle.js"></script>
				<?
				$sql = "SELECT 
							arqnome, arq.arqid, 
							arq.arqextensao, arq.arqtipo, 
							arq.arqdescricao,							
						 	to_char(arq.arqdata, 'DD/MM/YYYY') as data,
						 	'<img style=\"cursor:pointer; position:relative; z-index:10; top:-87px; left:-9px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:excluirFoto(\'" . $caminho_atual . "&requisicao=excluir" . "\',' || arq.arqid || ',' || pof.pofid || ');\">' as acao
						FROM 
							public.arquivo arq
						INNER JOIN 
							obras.preobrafotos pof ON arq.arqid = pof.arqid
						INNER JOIN 
							obras.preobra pre ON pre.preid = pof.preid
						--INNER JOIN 
							--seguranca.usuario seg ON seg.usucpf = oar.usucpf 
						WHERE 
							pre.preidsistema = {$_GET['icoid']}
						AND 
							pre.preid = {$_SESSION['par']['preid']} 
						AND							
							(arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
						ORDER BY 
							arq.arqid
						LIMIT 16 OFFSET ".($_REQUEST['pagina']*16);
//				ver($_SESSION['par'], d);
				$fotos = ($db->carregar($sql));				
				$_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "obras");				

				if( $fotos ){
					$_SESSION['imgparams'] = array("filtro" => "cnt.preid={$_SESSION['par']['preid']}", 
												   "tabela" => "obras.preobrafotos");
					//title=\"". $fotos[$k]["arqdescricao"] ."\"
					for( $k=0; $k < count($fotos); $k++ ){
						echo "<div style=\"float:left; width:90px; height:100px; text-align:center; margin:3px;\" >
								<img title=\"".$fotos[$k]["arqdescricao"]."\" border='1px' id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid=".$fotos[$k]["arqid"]."&_sisarquivo=obras' hspace='10' vspace='3' style='position:relative; z-index:5; float:left; width:70px; height:70px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&_sisarquivo=obras&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/><br>
								" . $fotos[$k]["data"] . " <br/>
								" . $fotos[$k]["acao"] . "											
							  </div>";
						
					}
					
				}else {
					echo "Não existem fotos cadastradas";
				}
				?>
			</td>
		</tr>					
		<tr>
			<td align="center">
				<?
				if(!$_REQUEST['pagina']) $_REQUEST['pagina'] = 0;
				$sql = "SELECT COUNT(arq.arqid) AS totalregistros FROM par.subacaotemporariafotos AS cnt 
						LEFT JOIN public.arquivo AS arq ON arq.arqid = cnt.arqid 
						WHERE subid = {$subid}  AND 
						(arqtipo = 'image/jpeg' OR
						 arqtipo = 'image/gif' OR
						 arqtipo = 'image/png')";
				$paginacao = current($db->carregar($sql));
				if($paginacao) {
					for($i = 0; $i < ceil(current($paginacao)/16); $i++ ) {
						$page[] = "<a href=?modulo=principal/album&acao=A&pagina=". $i .">".(($i==$_REQUEST['pagina'])?"<b>".($i+1)."</b>":($i+1))."</a>";
					}
					if(count($page) > 1) {
						echo implode(" | ", $page);
					}
				}
				?>
			</td>
		</tr>
	</table>
</form>