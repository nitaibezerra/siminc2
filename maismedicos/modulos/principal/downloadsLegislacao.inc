<?php 

include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$file = new FilesSimec();

if($_REQUEST['requisicao'] == 'downloadArquivo'){
	$file->getDownloadArquivo($_REQUEST['arqid']);
}

?>
<script>

function baixarArquivo( arqid )
{
	document.location.href = '/maismedicos/maismedicos.php?modulo=sistema/tabelasapoio/arquivo&acao=A&requisicao=downloadArquivo&arqid='+arqid;
}

</script>
<?php 

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print '<br/>';
monta_titulo( 'Legislação', '&nbsp;' );

$db->cria_aba($abacod_tela, $url, $parametros);


$sql = "select
			arq.arqid,
			anx.catid, 
			cat.catdsc,
		 	anx.arqdsc,
		 	arq.arqnome || '.' || arqextensao as arquivo,
			round( (arq.arqtamanho::numeric/1024)  , 2 ) || 'kb' as tamanho,
			arqextensao,
			arqtipo
		from maismedicos.arquivo anx
		join public.arquivo arq on arq.arqid = anx.arqid
		left join maismedicos.categoria cat on cat.catid = anx.catid		 
		where anx.arqstatus = 'A'
		and cat.legislacao = 't'
		and cat.catgrupo = 'arquivo'
		order by cat.catordem desc, anx.arqordem desc";

$rsLinks = $db->carregar($sql);


$arCategoriasProcessadas = array();
?>

<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
	<tr>
		<td>
		
			<?php if($rsLinks): ?>
			
				
						<?php foreach($rsLinks as $link): ?>
							<?php 
							if(!in_array($link['catid'], $arCategoriasProcessadas)){
								$arCategoriasProcessadas[$link['catid']] = $link['catid'];
								echo '<p>&nbsp;</p>';
								echo '<b><font size="3">'.$link['catdsc'].'</b></p>';
							}		
							?>
							<div style="padding:10px;">
							
								<?php 
								
								
								if(in_array($link['arqextensao'], array('pdf'))){

									$imagem_download = '../imagens/pdf.gif';
									
								}else if(in_array($link['arqextensao'], array('doc', 'docx', 'rtf'))){

									$imagem_download = '../imagens/icone_word.png';
									
								}else if(in_array($link['arqextensao'], array('xls', 'csv', 'xlsx'))){

									$imagem_download = '../imagens/excel.gif';
									
								}else if(in_array($link['arqextensao'], array('ppt', 'pptx'))){

									$imagem_download = '../imagens/icone_powerpoint.png';
									
								}else if(strpos( $link['arqtipo'], 'image' ) !== false){

									$imagem_download = '../imagens/obras/imagem.png';

								}else{

									$imagem_download = '../imagens/lupa_grafico.gif';								
	
								}
								
								?>
								
								<img height="15" src="<?php echo $imagem_download; ?>" style="cursor:pointer" onclick="baixarArquivo('<?php echo $link['arqid']; ?>')" title="Download" alt="Download" />
								<b><font size="2"><a href="javascript:void(0)" onclick="baixarArquivo('<?php echo $link['arqid']; ?>')"><?php echo $link['arquivo']; ?></a> - <?php echo $link['tamanho']; ?></font></b>
								<br/>					
								<font size="2"><b>Descrição:</b>&nbsp;<?php echo $link['arqdsc']; ?></font>
								<br/>
								<?php echo $link['lnkdsc']; ?>
							</div>
						<?php endforeach; ?>
						
						
				
			<?php else: ?>
						
				<center><font color="red"><b>Sem registros!</b></font></center>
			
			<?php endif; ?>

		</td>
	</tr>
</table>