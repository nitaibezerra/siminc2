<?
include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo "<br/>";
echo montarAbasArray(monta_abas_processo($_SESSION['profeinep_var']['tprid']),$_SERVER['REQUEST_URI']);
monta_cabecalho_profeinep($_SESSION['profeinep_var']['prcid']);

$docid = pegarDocid($_SESSION['profeinep_var']['prcid']);
if($docid) {
	$documento = wf_pegarDocumento( $docid );
	$atual = wf_pegarEstadoAtual( $docid );
	$historico = wf_pegarHistorico( $docid );
} else {
	direcionar('?modulo=inicio&acao=C','Problemas na criação do documento. Entre em contato com o suporte técnico.');
}

// verifica se muda a cor da aba de documento
$boMudaCorAba = verificaSeMudaCor($_SESSION['profeinep_var']['prcid']);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

	$(document).ready(function(){
	
		// Serve para mudar a cor da aba documentos, quando existe número gerado e não tem arquivo anexado
		var boMudaCorAba = '<?php echo $boMudaCorAba;?>';
		if(!boMudaCorAba){
			if($('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().text() != 'Documentos'){
				$('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().css('color','red').css('font-weight','bold');
			} else {
				$('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().css('color','red').css('font-weight','bold');
			}
			
		}
	});
	
	
	IE = !!document.all;
	
	function exebirOcultarComentario( docid )
	{
		id = 'comentario' + docid;
		div = document.getElementById( id );
		if ( !div )
		{
			return;
		}
		var display = div.style.display != 'none' ? 'none' : 'table-row';
		if ( display == 'table-row' && IE == true )
		{
			display = 'block';
		}
		div.style.display = display;
	}
	
</script>
<form action="" method="post" name="formulario">
	<table class="tabela" cellspacing="0" cellpadding="3" align="center">
		<thead>
			<tr>
				<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
					<b style="font-size: 10pt;">Histórico de Tramitações<br/></b>
					<div><?php echo $documento['docdsc']; ?></div>
				</td>
			</tr>
			<?php if ( count( $historico ) ) : ?>
				<tr>
					<td style="width: 20px;"><b>Seq.</b></td>
					<td style="width: 200px;"><b>Onde Estava</b></td>
					<td style="width: 200px;"><b>O que aconteceu</b></td>
					<td style="width: 90px;"><b>Quem fez</b></td>
					<td style="width: 120px;"><b>Quando fez</b></td>
					<td style="width: 17px;">&nbsp;</td>
				</tr>
			<?php endif; ?>
		</thead>
		<?php $i = 1; ?>
		<?php foreach ( $historico as $item ) : ?>
			<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
			<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
				<td align="right"><?=$i?>.</td>
				<td style="color:#008000;">
					<?php echo $item['esddsc']; ?>
				</td>
				<td valign="middle" style="color:#133368">
					<?php echo $item['aeddscrealizada']; ?>
				</td>
				<td style="font-size: 6pt;">
					<?php echo $item['usunome']; ?>
				</td>
				<td style="color:#133368">
					<?php echo $item['htddata']; ?>
				</td>
				<td style="color:#133368; text-align: center;">
					<?php if( $item['cmddsc'] ) : ?>
						<img
							align="middle"
							style="cursor: pointer;"
							src="http://<?php echo $_SERVER['SERVER_NAME'] ?>/imagens/restricao.png"
							onclick="exebirOcultarComentario( '<?php echo $i; ?>' );"
						/>
					<?php endif; ?>
				</td>
			</tr>
			<tr id="comentario<?php echo $i; ?>" style="display: none;" bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
				<td colspan="6">
					<div >
						<?php echo simec_htmlentities( $item['cmddsc'] ); ?>
					</div>
				</td>
			</tr>
			<?php $i++; ?>
		<?php endforeach; ?>
		<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
		<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
			<td style="text-align: right;" colspan="6">
				Estado atual: <span style="color:#008000;"><?php echo $atual['esddsc']; ?></span>
			</td>
		</tr>
	</table>
	<br><br>
	<table class="tabela" cellspacing="0" cellpadding="3" align="center">
		<thead>
			<tr>
				<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
					<b style="font-size: 10pt;">Histórico de Advogados<br/></b>
				</td>
			</tr>
			<?php 
				
				$sql = "SELECT 
							to_char(dtatribuicao,'DD/MM/YYYY - HH24:MI:SS') as data,
							ent.entnome as nome,
							coodsc as coord,
							CASE WHEN advstatus = 'A'
								THEN 'Sim'
								ELSE 'Não'
							END as ativo 
						FROM 
							profeinep.historicoadvogados had
						INNER JOIN profeinep.advogados  adv ON adv.advid = had.advid
						INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
						INNER JOIN profeinep.coordenacao coo ON coo.coonid = adv.coonid
						WHERE
							prcid = ".$_SESSION['profeinep_var']['prcid']."
						ORDER BY 1";
				
				$historicoAdv = $db->carregar($sql);
			?>
			<?php if ( count( $historicoAdv ) ) : ?>
				<tr>
					<td style="width: 4%;"><b>Seq.</b></td>
					<td style="width: 40%;"><b>Advogado</b></td>
					<td style="width: 30%;"><b>Coordenação</b></td>
					<td style="width: 14%;"><b>Ativo</b></td>
					<td style="width: 12%;"><b>Data</b></td>
				</tr>
			<?php endif; ?>
		</thead>
		<?php if( is_array( $historicoAdv ) ){?>
			<?php foreach ( $historicoAdv as $i => $item ) : ?>
				<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
				<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
					<td align="right"><?=$i+1?>.</td>
					<td style="color:#008000;">
						<?php echo $item['nome']; ?>
					</td>
					<td valign="middle" style="color:#133368">
						<?php echo $item['coord']; ?>
					</td>
					<td valign="middle" style="color:#133368">
						<?php echo $item['ativo']; ?>
					</td>
					<td valign="middle" style="color:#133368">
						<?php echo $item['data']; ?>
					</td>
				</tr>
			<?php endforeach; ?>
		<?php } ?>
		<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
	</table>
	<br><br>
	<table class="tabela" cellspacing="0" cellpadding="3" align="center">
		<thead>
			<tr>
				<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
					<b style="font-size: 10pt;">Histórico de Local Físico<br/></b>
				</td>
			</tr>
			<?php 
				
				$sql = "SELECT 
							to_char(hco.dtatribuicao,'DD/MM/YYYY - HH24:MI:SS') as data,
							hco.dtatribuicao,
							coodsc as coord
						FROM 
							profeinep.historicocoordenacoes hco
						INNER JOIN profeinep.coordenacao coo ON coo.coonid = hco.coonid
						WHERE
							prcid = ".$_SESSION['profeinep_var']['prcid']."
						ORDER BY hcoid";
				
				$historicoCoord = $db->carregar($sql);
			?>
			<?php if ( count( $historicoCoord ) ) : ?>
				<tr>
					<td style="width: 4%;"><b>Seq.</b></td>
					<td style="width: 74%;"><b>Coordenação</b></td>
					<td style="width: 22%;"><b>Data</b></td>
				</tr>
			<?php endif; ?>
		</thead>
		<?php if( is_array( $historicoCoord ) ){ ?>
			<?php foreach ( $historicoCoord as $i => $item ) : ?>
				<?php $marcado = $i % 2 == 0 ? "" : "#f7f7f7";?>
				<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
					<td align="right"><?=$i+1?>.</td>
					<td style="color:#008000;">
						<?php echo $item['coord']; ?>
					</td>
					<td valign="middle" style="color:#133368">
						<?php echo $item['data']; ?>
					</td>
				</tr>
			<?php endforeach; ?>
		<?php } ?>
		<?php $marcado = $i++ % 2 == 0 ? "" : "#f7f7f7";?>
		<tr bgcolor="<?=$marcado?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$marcado?>';">
			<td style="text-align: right;" colspan="6">
			<? $cooAtual = $db->pegaUm("SELECT coodsc FROM profeinep.coordenacao coo 
									    LEFT JOIN profeinep.processoprofeinep prc ON prc.cooid = coo.coonid 
									    WHERE prcid='".$_SESSION['profeinep_var']['prcid']."'"); ?>
				Unidade Atual: <span style="color:#008000;"><?php echo $cooAtual; ?></span>
			</td>
		</tr>
	</table>
</form>