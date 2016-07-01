<input type="hidden" name="req" value=""/>
<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' >
	<tr>
		<td colspan="5" style="font-size:12px;" >
			<?php 
			$param['link1'] 	= "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=9";
			$param['link2'] 	= "academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&questPg=9";
			$param['link2tipo'] = 5;
			montarAbasConsolidacao( $param );
			?>
		</td>
	</tr>
	<tr>
		<td style="font-size:12px;" >
<!--			<b>5. Demanda - Arquivos</b>-->
		</td>
	</tr>
	<?php 
	//Inicio Lista Questões
	$sql = "SELECT 
				perid, 
				pernumero, 
				pertitulo
			FROM 
				academico.pergunta 
			WHERE
				pernumero ilike '4.1.%'";
	
	$item = $db->pegaLinha($sql);
	?>
	<tr>
		<td style="font-size:12px;" >
			<input type="hidden" name="perid" value="<?=$item['perid'] ?>"/>
			<input type="hidden" name="tabela" value="true"/>
			<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center' style="width:100%;" >
				<tr>
					<td bgcolor="#DCDCDC" align="center" width="50%"><b>Demanda</b></td>
					<td bgcolor="#DCDCDC" align="center"><b>Área[m²]</b></td>
				</tr>
				<?php 
				$sql = "SELECT
							tbtid,
							tbttipo
						FROM
							academico.tabelatipo
						WHERE
							tbtid NOT IN (6, 7)";
				$tipos = $db->carregar($sql);
				foreach($tipos as $tipo){
					$sql = "";
				?>
				<tr>
					<td colspan="12" bgcolor="white"><b><?=$tipo['tbttipo'] ?></b></td>
				</tr>
				<?php 
				
				$sql = "SELECT
							tbrid as linha,
							tbrdemanda,
							arqid
						FROM
							academico.tabelaresposta
						WHERE
							tbrprojeto IS TRUE
							AND perid = ".$item['perid']."
							AND entid = ".$_SESSION['academico']['entidcampus']."
							AND tbtid = ".$tipo['tbtid']."
						ORDER BY
							1";
				$linhas = $db->carregar($sql);
				$linhas = $linhas ? $linhas : Array();
				foreach( $linhas as $linha ){
				?>
				<tr>
					<td>
						<b><?=$linha['tbrdemanda'] ?></b>
					</td>
					<td>
						<?php 
						if( $linha['arqid'] ){
						?>
						<a class="baixar" style="cursor:pointer" id="<?=$linha['arqid']?>">Clique aqui para baixar</a>
						<?php if( $bloq == 'S' ){ ?>
						<img border="0" src="../imagens/excluir_2.gif" width="12px" class="excluir" title="Excluir Arquivo." id="<?=$linha['linha']?>" style="cursor:pointer;">
						<?php }?>
						<?php 
						}else{
						?>
						<input type="file" class=" normal" name="arquivo_<?=$tipo['tbtid'] ?>_<?=$linha['linha']?>" style="text-align:left;">
						<?php 
						}
						?>
					</td>
				</tr>
				<?php 
				}
				}
				?>
			</table>
		</td>
	</tr>
</table>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
$('.excluir').live('click',function(){
	window.location = 'academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&req=excluirArquivo2&pg=9&tbrid='+$(this).attr('id');
});
$('.baixar').live('click',function(){
	return window.open('academico.php?modulo=principal/consolidacaoExpansao/consolidacaoExp_questCamp&acao=A&arqid='+$(this).attr('id'), 
			   'ajuda', 
			   "height=300,width=450,scrollbars=yes,top=50,left=200" );
});
</script>