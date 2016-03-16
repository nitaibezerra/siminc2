<?php 
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec();
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'livro.php?modulo=principal/pnld_processos&acao=A';</script>";
    exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br />";
$db->cria_aba($abacod_tela,$url,$parametros);

monta_titulo('Processos',$linha2);

$sql = "SELECT DISTINCT
			c.coltitulo, c.colid
		FROM
			livro.colecao c
		    inner join livro.resenharecurso rr on rr.colid = c.colid
		WHERE
			rr.rrestatus = 'A'
		    and c.colstatus = 'A'";

$arrColecao = $db->carregar( $sql );
$arrColecao = $arrColecao ? $arrColecao : array();
?>
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
						<a href="?#">Fechar Todos</a> | <a href="?#" id="abrirTodos">Abrir Todos</a> 
						<br /><br /> 
						<img src="../imagens/livro.gif" align="top" width="15px"/>&nbsp;<strong>Processos</strong>
					</div>
					<ul id="tree" class="filetree treeview-famfamfam">
					<?
					foreach ($arrColecao as $key => $colecao) {					
					?>
						<li>
							<span class="folder">&nbsp;<?=$colecao['coltitulo']; ?></span>
							<ul>
								<?
								$sql = "SELECT
											rr.rreid,
											rr.arqid,
										    rr.rredescricao
										FROM
											livro.resenharecurso rr
										WHERE
										rr.rrestatus = 'A'
									    and rr.colid = ".$colecao['colid'];
								$arrProcesso = $db->carregar( $sql );
								$arrProcesso = $arrProcesso ? $arrProcesso : array();
								
								foreach ($arrProcesso as $processo) {?>
									<li>
										<span> 
											<img src="../imagens/anexo.gif" onclick="window.location='?modulo=principal/pnld_processos&acao=A&download=S&arqid=<?php echo $processo['arqid'];?>'" style="cursor: pointer" align="top" border="0" /> <?=$processo['rredescricao']; ?>
										</span>
									</li>
								<?} ?>
							</ul>
						</li>
					<?} ?>
					</ul>
				</div>
			</div>
		</td>
	</tr>
</table>