<?php 
if( $_REQUEST['lvdid'] ){
	
	$_REQUEST['lvdtitulo'] 			= str_replace("\'","",$_REQUEST['lvdtitulo']);
	$_REQUEST['lvdcodigo'] 			= str_replace("\'","",$_REQUEST['lvdcodigo']);
	$_REQUEST['lvdtipo'] 			= str_replace("\'","",$_REQUEST['lvdtipo']);
	$_REQUEST['lvdanoedicao'] 		= str_replace("\'","",$_REQUEST['lvdanoedicao']);
	$_REQUEST['lvdnumeropaginas']   = str_replace("\'","",$_REQUEST['lvdnumeropaginas']);
	$_REQUEST['lvdautor'] 			= str_replace("\'","",$_REQUEST['lvdautor']);
	
	if( $_REQUEST['lvdtipo'] == 'L' ){ $tipo = 'Livro do Aluno'; }
	if( $_REQUEST['lvdtipo'] == 'M' ){ $tipo = 'Manual do Professor'; }
?>
<table>
	<tr>
		<td>
			Titulo: 
		</td>
		<td>
			<?=$_REQUEST['lvdtitulo'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Código:
		</td>
		<td>
			<?=$_REQUEST['lvdcodigo'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Tipo:
		</td>
		<td>
			<?=$tipo ?>
		</td>
	</tr>
	<tr>
		<td>
			Ano Edição:
		</td>
		<td>
			<?=$_REQUEST['lvdanoedicao']; ?>
		</td>
	</tr>
	<tr>
		<td>	
			Numero de Paginas:
		</td>
		<td>
			<?=$_REQUEST['lvdnumeropaginas'] ?>
		</td>
	</tr>
	<tr>
		<td>
			Autor:
		</td>
		<td>
			<?=$_REQUEST['lvdautor'] ?>
		</td>
	</tr>
</table>

<?php 
	exit;
}
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
$db->cria_aba($abacod_tela,$url,$parametros);

monta_titulo('Livro Didático',$linha2)

?>
<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />
<script type="text/javascript" src="../includes/jquery-treeview/lib/jquery.cookie.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview-simec.js"></script>
<script type="text/javascript">

var janela;

$(document).ready(function(){
	
	$("#_arvore").treeview({
		collapsed: true,
		animated: "medium",
		control:"#sidetreecontrol",
		persist: "cookie"
	});

	$('#abrirTodos').click();

	$(window).unload(function() {
		janela.close();
	});

});

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr>
		<td width="100%" valign="top" style="background: none repeat scroll 0% 0%;" id="_arvore">
			<div id="main" style="background: #f5f5f5">
				<div id="sidetree">
					<div class="treeheader">&nbsp;</div>
					<div id="sidetreecontrol">
						&nbsp;&nbsp;&nbsp;&nbsp;
						<a href="?#">Fechar Todos</a> | <a href="?#" id="abrirTodos">Abrir Todos</a> 
						<br /><br /> 
						&nbsp;<img src="../imagens/livro.gif" align="top" width="15px"/>&nbsp;<strong>Livro Didático</strong></div>
						<ul id="tree" class="filetree treeview-famfamfam">
							<li>
								<span class="folder" >
									&nbsp;Coleção - Código
								</span>
								<ul>
<?php 

						if(!$db->testa_superuser()){
							$arrPerfil = arrayPerfil();
					    	if(in_array( PNLD_EDITORA, $arrPerfil )) {
					    		$inner = "INNER JOIN livro.usuarioresponsabilidade ur ON ur.edtid=col.edtid AND rpustatus='A' AND ur.usucpf='".$_SESSION['usucpf']."' AND pflcod='".PNLD_EDITORA."'";
					    	}
						}


						$sql = "SELECT 
									col.colid,
									col.coltitulo || ' - ' || col.colcodigo as colecao,
									lid.lvdtitulo || ' - ' || lid.lvdid as livro,
									lid.lvdid as livroid,
									lvdtitulo,
									lvdcodigo,
									lvdtipo,
									lvdanoedicao,
									lvdnumeropaginas,
									lvdautor	
								FROM 
									livro.colecao col
								INNER JOIN
									livro.livrodidatico lid ON lid.colid = col.colid 
								{$inner}
								WHERE
									col.colstatus = 'A'
								ORDER BY
									colecao,livro";
						$dados = $db->carregar($sql);
						$dados = $dados ? $dados : array();
						
						$colecao = 0;
						foreach($dados as $livro){
							if( $livro['colid'] != $colecao ){
								if( $colecao != 0 ){ 
									echo "</ul></li>";
								}
								echo "<li>
										<span class=\"folder\">
											<a title=\"Resenhas desta coleção.\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid=".$livro['colid']."\">
												&nbsp;".$livro['colecao']."
											</a>
										</span>
										<ul>";
								$colecao = $livro['colid'];
							}
							echo "			<li>
												<span> 
													<a href=\"livro.php?modulo=principal/pnld_folha_de_rosto&acao=A&lvdid=".$livro['livroid']."\">
														<img src=\"../imagens/alterar.gif\" title=\"Editar informações do Livro\" style=\"cursor: pointer\" align=\"top\" border=\"0\"/>&nbsp;
														<label onmouseover=\"SuperTitleAjax('livro.php?modulo=principal/pnld_livro_didatico&acao=A&lvdid=".$livro['livroid']."&lvdtitulo=\'".$livro['lvdtitulo']."\'&lvdcodigo=".$livro['lvdcodigo']."&lvdtipo=\'".$livro['lvdtipo']."\'&lvdanoedicao=".$livro['lvdanoedicao']."&lvdnumeropaginas=".$livro['lvdnumeropaginas']."&lvdautor=\'".$livro['lvdautor']."\'')\"> ".$livro['livro']."</label>
													</a>
												</span>
											</li>";
						}
?>
								</ul>
							</li>
						</ul> 
				</div>
			</div>
		</td>
	</tr>
</table>