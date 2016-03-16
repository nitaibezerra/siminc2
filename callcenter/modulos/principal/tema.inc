<?php
unset( $_SESSION['callcenter']['temid'] );
$sql = "SELECT temid, sisid, temdatainicio, temdatafim, temaimagem, temalink, temadatainclusao, temastatus 
		FROM callcenter.tema ORDER BY temid";
$listaDeTemas = $db->carregar($sql);
?>
<style>
#quadro
{
}
.botao
{
	padding: 20px;
	float: left;
	cursor: pointer;
	margin: 5px;
	border: solid black 1px;
	background-color: #f5f5f5;
}
</style>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="5" cellPadding="5" align="center" >
	<tr>
		<td>
			<div id="quadro">
			<?php  
				foreach($listaDeTemas as $temas){
			?>
			
				<div id="<?php echo $temas['temid']; ?>" class="botao" style="height: 100px;">
					<img onclick="abretema( '<?php echo $temas['temalink'].'&temid='.$temas['temid']; ?> ');" src="<?php echo $temas['temaimagem']; ?>" />
				</div>
			<?php } ?>
			</div>
		</td>
	</tr>
</table>
<script>
	function abretema(link){
			window.location.href = link;
			/*
			window.open(link, 'pac', 
				    "height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
				    */
	}
</script>