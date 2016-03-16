<?php 

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
print '<br/>';
monta_titulo( 'Educação Permanente', '&nbsp;' );

$db->cria_aba($abacod_tela, $url, $parametros);


$sql = "select
			lnk.catid, 
			cat.catdsc,
		 	lnk.lnktitulo,
		 	lnk.lnksubtitulo,
			lnk.lnkdsc,
			lnk.lnkurl
		from maismedicos.link lnk
		left join maismedicos.categoria cat on cat.catid = lnk.catid		 
		where lnkstatus = 'A'
		and educacaopermanente = true
		order by cat.catordem desc, lnk.lnkordem desc";

$rsLinks = $db->carregar($sql);


$arCategoriasProcessadas = array();
?>

<?php if($rsLinks): ?>

	<table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td>
			<?php foreach($rsLinks as $k => $link): ?>
				<?php 
				if(!in_array($link['catid'], $arCategoriasProcessadas)){
					$arCategoriasProcessadas[$link['catid']] = $link['catid'];
					echo $k>1 ? '<p>&nbsp;</p>' : '';
					echo '<p><b><font size="3">'.$link['catdsc'].'</font></b></p>';
				}		
				?>
				<div style="padding:10px;">
					<b><font size="2"><?php echo $link['lnktitulo']; ?></font></b><br/>
					<a href="<?php echo $link['lnkurl']; ?>" target="_blank"><img src="../imagens/globo_terrestre.png" alt="Abrir link" title="Abrir link" /></a>
					<b><font size="2" title="Abrir link" alt="Abrir link"><a href="<?php echo $link['lnkurl']; ?>" target="_blank"><?php echo $link['lnksubtitulo'] ? $link['lnksubtitulo'] : $link['lnkurl']; ?></a></font></b>
					<br/>
					<?php echo $link['lnkdsc']; ?>
				</div>
			<?php endforeach; ?>
			
			</td>
		</tr>
	</table>

<?php else: ?>
						
	<center><font color="red"><b>Sem registros!</b></font></center>

<?php endif; ?>