<?php
$eveid 						 = ($_GET['eveid'] ? $_GET['eveid'] : $_SESSION['agenda']['eveid']);
$_SESSION['agenda']['eveid'] = $eveid;
$agdid 						 = $_SESSION['agenda']['agdid'];

if ( empty($eveid) ){
	die("<script>
			alert('Faltam parametros para acessar a tela!');
			window.close();
		 </script>");
}

$evaid = ( $_REQUEST['evaid'] ? $_REQUEST['evaid'] : $_SESSION['agenda']['evaid']);
if( $evaid ){ 
	$docid = pegaDocidEventoArea($evaid);
	$esdid = pegaEstadoEventoArea( $docid );
	if ( !in_array( $esdid, $_ESTADOS_WF_AGENDA_UNIDADE ) ){
		$habilNovoParecer = 'N';		
	}
}
?>
<html>
	<head>
		<title>Relatório para Despacho Inicial</title>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
		<script>
		//window.print();
		</script>
		<style>
		<!--
		.tb_rel_despacho div{
			/*border: 1px solid red;*/
			padding: 2px;
		}
		-->
		</style>
	</head>	
<body marginheight="0" marginwidth="0">
<?php 
$agenda   = new Agenda();
$dadoResp = $agenda->carregaResponsavel( $agdid );

$evento = new Evento();
$dados  = $evento->carregaTodosDados( $eveid );

include APPRAIZ . 'includes/workflow.php';

$documento = wf_pegarDocumento( $docid );
$atual = wf_pegarEstadoAtual( $docid );
$historico = wf_pegarHistorico( $docid );
?>
<table width="95%" align="center" class="notprint">
	<tr>
		<td align="right">
			<img align="absmiddle" src="../imagens/ico_print.jpg" style="cursor: pointer" onclick="javascript: window.print();" title="Imprimir Despacho Inicial">
		</td>
	</tr>
</table>
<table class="tb_rel_despacho" align="center" style="border-style:solid;" width="95%" border="1" bgcolor="#FFF">
	<thead>
		<td style="border: 0px; border-bottom: 1px double #000000;">
			<div style="float: right; margin-right: 2px; font-weight: bold;">
			<?php 
			echo "Data:&nbsp;" . date('d/m/Y') . "<br>";
			echo "Hora:&nbsp;" . date('H:i') . "<br>";
//			echo "Pág.:&nbsp;&nbsp;1<br>";
			?>
			</div>
			<div style="float:left; height: 50px; margin-right: 3px; width: 30px;">
			MEC
			</div>
			<div style="font-weight: bold;">
			Ministério da Educação
			</div>
			<div style="font-weight: bold;">
			Chefia de Gabinete do Ministro
			</div>
			<div style="font-weight: bold;">
			Solicitação de Agenda(SA) e Controle de Documentos(CD)
			</div>
		</td>
	</thead>
	<tr>
		<td style="border: 1px solid #000000; border-right: 0px; border-left: 0px; font-weight: bold;" align="center">
			RELATÓRIO PARA DESPACHO INICIAL		
		</td>
	</tr>
	<tr>
		<td style="border-top: 1px solid #000000; border-left: 0px; border-right: 0px; border-bottom: 0px;  font-size:10pt;">
			<div style="float:left; font-weight: bold; margin-right:2px; width: 110px; text-align: right;">Registro: </div><div style="float: left; width: 120px;"><?php echo $dados['eveid'] . '/' . $dados['anoevento'] ?></div>
			<div style="float:left; font-weight: bold; margin-left:30px; margin-right:2px;">Evento: </div><div style="float: left; width: 100px;"><?php echo $dados['tpedsc']; ?></div>
			<div style="float:left; font-weight: bold; margin-left:30px; margin-right:2px;">Prioridade: </div><div style="float: left;"><?php echo $dados['tppdsc']; ?></div>
			<div style="float:left; font-weight: bold; margin-left:30px; margin-right:2px;">Situação: </div><div style="float: left;">?????</div>
	
			<div style="clear: left;  float:left; font-weight: bold; margin-right:2px; width: 110px; text-align: right;">UF: </div><div style="float: left; width: 120px;"><?php echo $dados['estuf']; ?></div>
			<div style="float:left; font-weight: bold; margin-left:30px; margin-right:2px;">Cidade: </div><div style="float: left; width: 100px;"><?php echo $dados['mundescricao']; ?></div>
			<div style="float:left; font-weight: bold; margin-left:30px; margin-right:2px;">Responsável: </div><div style="float: left; width: 260px;"><?php echo $dados['responsavel']; ?></div>
			
			<div style="clear: left; float:left; font-weight: bold; margin-right:2px; width: 110px; text-align: right;">Data: </div><div style="float: left; width: 120px;"><?php echo $dados['evedtinicio']; ?> <b>a</b> <?php echo $dados['evedtfinal']; ?></div>
			<div style="float:left; font-weight: bold; margin-left:30px; margin-right:2px;">Data Cadastro: </div><div style="float: left; width: 100px;"><?php echo $dados['evedtinclusao']; ?></div>
			
			<div style="clear: left;  float:left; font-weight: bold; margin-right:2px; width: 110px; text-align: right;">Horário de Início: </div><div style="float: left; width: 120px;"><?php echo $dados['evehsinicio']; ?></div>
			<div style="float:left; font-weight: bold; margin-left:30px; margin-right:2px;">Horário de Término: </div><div style="float: left; width: 100px;"><?php echo $dados['evehsfinal']; ?></div>
			
			<div style="clear: left; float:left; font-weight: bold; margin-right:2px; width: 110px; text-align: right;">Solicitante: </div><div style="float: left;"><?php echo $dados['sltdsc']; ?> - <?php echo $dados['sltcargo']; ?></div>
			
			<div style="clear: left; float:left; font-weight: bold; margin-right:2px; width: 110px; text-align: right;">Entidade: </div><div style="float: left;"><?php echo $dados['etadsc']; ?></div>
			
			<div style="clear: left; float:left; font-weight: bold; margin-right:2px; width: 110px; text-align: right;">Assunto: </div><div style="float: left;"><?php echo nl2br( $dados['eveassunto'] ); ?></div>
			<div style="clear: left; float:left; font-weight: bold; margin-right:2px; width: 110px; text-align: right;">&nbsp;</div>
			<div style="float:left; margin-top:15px;">
			Data: <?php echo $dados['evedtinicio']; ?>, ás <?php echo $dados['evehsinicio']; ?>.
			<br>
			Local: <?php echo $dados['evebairro']; ?> - <?php echo $dados['mundescricao']; ?> - <?php echo $dados['estuf']; ?>.
			<br>
			Doc.: ????
			</div>
		</td>
	</tr>
	<tr>
		<td style="background:#E9E9E9; border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 0px; border-right: 0px; text-align: center;">
			<b>CONTATOS</b>
		</td>
	</tr>
	<tr>
		<td style="border: 0px;">
			<div style="float: left; font-weight: bold; border-bottom: 1px solid #000000; width: 38%; margin-right:10px;">Nome</div>
			<div style="float: left; font-weight: bold; border-bottom: 1px solid #000000; width: 27%; margin-right:10px;">Cargo</div>
			<div style="float: left; font-weight: bold; border-bottom: 1px solid #000000; width: 30%; margin-right:10px;">Entidade</div>
			<?php 
			$eventoContato = new EventoContato();
			$dadosContato = $eventoContato->listaPorEvento( $eveid );
			do{
				$dadosCont = current($dadosContato);
			?>
			<div style="clear: left; float: left; width: 39%;"><?php echo $dadosCont['cevdsc'] ?></div>
			<div style="float: left; width: 30%;"><?php echo $dadosCont['cevcargo'] ?></div>
			<div style="float: left; width: 30%;"><?php echo $dados['etadsc']; ?></div>
			<div style="clear: left; float: left; width: 55px;">TELEFONE:</div>
			<div style="float: left; width: 100px;"><?php echo $dadosCont['fone']; ?></div>
			<div style="float: left; width: 38px;">TRAB.:</div>
			<div style="float: left; width: 80px;"><?php echo $dadosCont['cel']; ?></div>
			<div style="clear: left; float: left; width: 45px;">E-MAIL:</div>
			<div style="float: left; width: 300px;"><?php echo $dadosCont['cevemail']; ?></div>
			<?php 
			if ( next($dadosContato) ):
				prev($dadosContato);
			?>
			<div style="float: left; width: 100%; border-bottom: 1px solid #000000;"></div>
			<?php 
			endif;
			?>
			<?php 
			}while (next($dadosContato));
			?>
		</td>
	</tr>
	<tr>
		<td style="background:#E9E9E9; border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 0px; border-right: 0px; text-align: center;">
			<b>DESPACHO</b>
		</td>
	</tr>
	<tr>
		<td style="border: 0px;">
			<div style="border-bottom: 1px solid #000000; text-align: center; font-weight: bold;"><i>Agendar:</i></div>
			<div style="float:left; margin-right: 15px;">Para: ______/______/__________</div>
			<div style="float:left;">Hora: ____________ a ____________</div>
			<div style="clear: left; float: left;">Participantes:</div>
			<div style="float: left; border-bottom: 1px solid #000000; width: 90%;">&nbsp;</div>
			<div style="clear: left; float: left; visibility: hidden;">Participantes:</div>
			<div style="float: left; border-bottom: 1px solid #000000; width: 90%;">&nbsp;</div>
			<div style="clear: left; float: left;">Aceito por:</div>
			<div style="float: left; border-bottom: 1px solid #000000; width: 40%;">&nbsp;</div>
			<div style="float: right; border: 1px solid #000000; width: 80px; margin-top:3px;">Agenda Internet</div>
			<div style="clear: both; float: right; border: 1px solid #000000; width: 37px; text-align: center;">Sim</div>
			<div style="float: right; border: 1px solid #000000; width: 37px; text-align: center;">Não</div>
			<div style="clear: left; float: left;">Avisado por:</div>
			<div style="float: left; border-bottom: 1px solid #000000; width: 39%;">&nbsp;</div>
		</td>
	</tr>
	<tr>
		<td style="font-weight:bold; border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 0px; border-right: 0px; text-align: center;">
			<i>Encaminhar/Agradecer/Arquivar</i>
		</td>
	</tr>
	<tr>
		<td style="border: 0px;">
			<div style="float:left; margin-right: 15px;">Por: Telefone/Telegrama/E-mail</div>
			<div style="clear: left; float:left;">Para: ________________________________________________________________________________________________</div>
			<div style="float:left;">Cargo: ________________________________________________________________________________________________</div>
			<div style="clear: left; float:left;">Órgão: __________________________________________________________________________________________________________________________________________</div>
			<div style="float:left;">Data: ______/______/__________</div>
		</td>
	</tr>
	<tr>
		<td valign="bottom" style="border-top: 1px solid #000000; border-bottom: 1px solid #000000; border-left: 0px; border-right: 0px; height: 250px;">
		<div style="float: right; text-align: center;">
		___________________________________________
		<br>
		<b><?php echo $dadoResp['agdnomeresp'] ?></b>
		<br>
		<?php echo $dadoResp['agdcargoresp'] ?>
		</div>
		</td>
	</tr>
	<tr>
		<td valign="bottom" style="border:0px; height: 150px;">
		<div style="float:left; text-align:center; margin-bottom:20px; font-weight: bold;">
		Histórico:<br />
		
		<?php
		if( $evaid ){
		?>
			<table class="listagem" cellspacing="0" cellpadding="3" align="center" style="width: auto;">
				<thead>
					<tr>
						<td style="text-align: center; background-color: #e0e0e0;" colspan="6">
							<b style="font-size: 10pt;">Histórico de Tramitações<br/></b>
						</td>
					</tr>
					<?php if ( count( $historico ) ) : ?>
						<tr>
							<td style="width: 10%;"><b>Seq.</b></td>
							<td style="width: 30%;"><b>Onde Estava</b></td>
							<td style="width: 30%;"><b>O que aconteceu</b></td>
							<td style="width: 10%;"><b>Quem fez</b></td>
							<td style="width: 15%;"><b>Quando fez</b></td>
							<td style="width: 5%;">&nbsp;</td>
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
			<?php 
			}else{
				echo '<br /> Não existe histórico de tramitação para o evento.';			
			}
			?>
		</div>
		</td>
	</tr>
</table>
</body>
</html>