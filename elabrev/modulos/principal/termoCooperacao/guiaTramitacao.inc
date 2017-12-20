<?php
ini_set( "memory_limit", "1024M" ); // ...
set_time_limit(0);

extract($_REQUEST);

if($grpid){

	$sql = "SELECT DISTINCT
				tcp.tcpid as n_do_processo,
				tcp.tcpdscobjetoidentificacao as assunto,
				'??' as campus,
				ung.ungcod as ug,
				gescod as gestao,
				a.acacod as acao--,
				--varios
				--'300300' as evento,
				--'1' as esfera,
				--ptr.ptres as ptres,
				--'??' as fonte,
				--pli.plicod as pi,
				--substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as nd,
				--pro.provalor as valor_nd
			FROM
				monitora.tramitalote_itensgrupo igr
			INNER JOIN workflow.historicodocumento		hst ON hst.hstid = igr.hstid
			INNER JOIN workflow.documento			doc ON doc.docid = hst.docid
			INNER JOIN monitora.termocooperacao		tcp ON tcp.docid = doc.docid 
			INNER JOIN monitora.previsaoorcamentaria 	pro ON pro.tcpid = tcp.tcpid
			LEFT  JOIN monitora.ptres 			  p ON   p.ptrid = pro.ptrid
			LEFT  JOIN monitora.acao 			  a ON   a.acaid =   p.acaid
			LEFT  JOIN monitora.pi_planointerno		pli ON pli.pliid = pro.pliid
			LEFT  JOIN monitora.pi_planointernoptres 	pts ON pts.pliid = pli.pliid
			LEFT  JOIN monitora.ptres			ptr ON ptr.ptrid = pts.ptrid
			LEFT  JOIN public.naturezadespesa 		ndp ON ndp.ndpid = pro.ndpid
			LEFT  JOIN public.unidadegestora 		ung ON ung.ungcod = tcp.ungcodproponente
			WHERE 
				grpid = $grpid";
	//dbg($sql);
	$dados = $db->carregar( $sql );
}

if($dados){
	
	foreach( $dados  as $k => $dado ){
		
		//$cabecalho = Array('N° do processo','Assunto','Campus','Unidade Gestora','Gestão','Ação');
		
		$tcpid = $dado['n_do_processo'];
		
		if($tcpid){
			/*
			$sql = "SELECT DISTINCT
						tcp.tcpid as n_do_processo,
						tcp.tcpdscobjetoidentificacao as assunto,
						'??' as campus,
						ung.ungcod as ug,
						gescod as gestao,
						a.acacod as acao,
						--varios
						'300300' as evento,
						'1' as esfera,
						ptr.ptres as ptres,
						'??' as fonte,
						pli.plicod as pi,
						substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as nd,
						pro.provalor as valor_nd
					FROM
						monitora.termocooperacao tcp 
					INNER JOIN monitora.previsaoorcamentaria 	pro ON pro.tcpid = tcp.tcpid
					LEFT  JOIN monitora.ptres 			  p ON   p.ptrid = pro.ptrid
					LEFT  JOIN monitora.acao 			  a ON   a.acaid =   p.acaid
					LEFT  JOIN monitora.pi_planointerno		pli ON pli.pliid = pro.pliid
					LEFT  JOIN monitora.pi_planointernoptres 	pts ON pts.pliid = pli.pliid
					LEFT  JOIN monitora.ptres			ptr ON ptr.ptrid = pts.ptrid
					LEFT  JOIN public.naturezadespesa 		ndp ON ndp.ndpid = pro.ndpid
					LEFT  JOIN public.unidadegestora 		unp ON ung.ungcod = tcp.ungcodproponente
					LEFT  JOIN public.unidadegestora 		unc ON ung.ungcod = tcp.ungcodconcedente
					WHERE 
						tcp.tcpid = $tcpid";
			*/
			
			/*
			   Coluna                    	Providências
			Unid Gestora       >>    Campo exibido na (aba Proponente) PDF-1. Cód. Und. Gestora
			Unid Gestão        >>    Campo exibido na (aba Proponente) PDF-2. Cód. da Gestão
			UG CONCEDENTE      >>    Campo exibido na (aba Objeto/Justificativa) PDF-47. UG/Gestão Repassadora
			GESTAO CONCEDENTE  >>    Campo exibido na (aba Objeto/Justificativa) PDF-47  UG/Gestão Recebedora
			Número original    >>    Corresponde ao número do TC (aba Proponente) Número do Termo: -PDF-
			TIPO DE CADASTRO   >>    fixo=6 (6 - Termo de Execução Descentralizado)
			Responsável 1      >>    CPF do Responsável pela UG concedente (aba Concedente) PDF-33. CPF
			Número do Processo >>    Corresponde ao número do TC (aba Proponente) Número do Termo: -PDF-
			Ug Recebedora      >>    Código Unidade Gestora (aba Proponente) PDF-1. Cód. Und. Gestora
			Gestão Recebedora  >>    Código da Gestão (aba Proponente) PDF-2. Cód. da Gestão
			Responsável 2      >>    CPF do Responsável pela UG Recebedora (aba Proponente) PDF-11. CPF
			Título             >>    Título do Termo de Execução Descentralizado - 70 caracteres (aba Objeto/Justificativa) PDF-45. Identificação (Título/Objeto da Despesa)
			Início da Vigência >>    Data de aprovação do Represente Legal Concedente (Histórico Workflow), formato: 99/MMM/9999, EX: 26/MAR/2013
			Fim da Vigência    >>    Data vigencia + Prazo (em meses) para o cumprimento do objeto (aba previsão orçament) = fim de Vigência
			                         transformar prazo de meses p/ dias e somar com data inicio da vigência formato: 99/MMM/9999, EX: 26/MAR/2013 ???de todas as ações???
			Valor firmado      >>    Valor da Programação orçamentária (total de NC) PDF-58. TOTAL
			Valor total        >>    Valor da Programação orçamentária (total de NC) PDF-58. TOTAL (inteiro, retirar o ponto decimal, deixar os centavos)
			Descrição          >>    Campo exibido na (aba Objeto/Justificativa) reduzir p/ 490 caracteres PDF-46. Objetivo
			Justificativa      >>    Campo exibido na (aba Objeto/Justificativa) reduzir p/ 350 caracteres PDF-49. Justificativa (Motivação/Clientela/Cronograma Fisico)
			Exercício 1        >>    Ano exercicio (SIMEC)
			Exercício 2        >>    Ano exercicio (SIMEC) + Prazo (em meses) para o cumprimento do objeto (aba previsão orçament)
			                         somar meses (prazo) ao ano exercício (SIMEC) e identificar o ano de encerramento, este(s) ano(s) maior(es) que o ano de exercício
			                         será(ão) preenchidos no campos exercício 2... até exercício 6
			Exercício 3        >>    ídem
			Exercício 4        >>    ídem
			Exercício 5        >>    ídem
			Exercício 6        >>    ídem
			Prazo 1            >>    fixo = 360
			Prazo 2            >>    nulo
			Prazo 3            >>    nulo
			Prazo 4            >>    nulo
			Prazo 5            >>    nulo
			(%) Real           >>    fixo = 10000
			
			   
			             Dados Variáveis: 1...8 (aba Previsão Orçamentária)
			-------------------------------------------
			Evento-1           >>    Fixo=300300        
			Esfera-1           >>    Deixar em branco
			PTRES-1            >>    6 primeiros dígitos do Programa de Trabalho (aba Previsão Orçamentária)
			Fonte-1            >>    Deixar  em branco
			PI-1               >>    Plano Interno  (aba Previsão Orçamentária)
			Natureza Despesa-1 >>    6 primeiros dígitos da Nat.Despesa (aba Previsão Orçamentária)
			Valor-1            >>    Valor (aba Previsão Orçamentária) (inteiro, retirar o ponto deciamal, deixar os centavos)
			N° de Registro     >>    Sequêncial da NC no lote de envio
			----------------------------------------
			 */
			
			$cabecalho = Array(	'UG PROPONENTE','GESTÃO PROPONENTE',
							   	'UG CONCEDENTE','GESTÃO CONCEDENTE',
							   	'NÚMERO ORIGINAL','TIPO DE CADASTRO',
							   	'RESPONSÁVEL 1','NÚMERO DO PROCESSO',
							   	'UG RECEBEDORA', 'GESTÃO RECEBEDORA',
							   	'RESPONSÁVEL 2','TÍTULO',
							   	'INÍCIO DA VIGÊNCIA','FIM DA VIGÊNCIA',
							   	'VALOR FIRMADO','VALOR TOTAL',
							   	'DESCRIÇÃO','JUSTIFICATIVA',
							   	'EXERCÍCIO 1','EXERCÍCIO 2',
								'EXERCÍCIO 3','EXERCÍCIO 4',
								'EXERCÍCIO 5','EXERCÍCIO 6',
							  	'PRAZO 1','PRAZO 2',
								'PRAZO 3','PRAZO 4',
								'PRAZO 5','(%) REAL');
			
			$sql = "SELECT DISTINCT
						tcp.ungcodproponente as ugproponente,
						unp.gescod as gescodproponente,
						tcp.ungcodconcedente as ugconcedente,
						unc.gescod as gescodconcedente,
						tcp.tcpid as nuoriginal,
						'6' as tipocadastro,
						tcp.ungcodconcedente as responsavel1,
						tcp.tcpid as nuprocesso,
						tcp.ungcodproponente as ugrecebedora,
						unp.gescod as gescodrecebedora,
						tcp.ungcodproponente as responsavel2,
						tcp.tcpdscobjetoidentificacao as titulo,
						'' as inivigencia,
						'' as fimvigencia,
						'' as valorfirmado,
						'' as valortotal,
						tcp.tcpobjetivoobjeto as descricao,
						tcp.tcpobjetojustificativa,
						'' as exercicio1,
						'' as exercicio2,
						'' as exercicio3,
						'' as exercicio4,
						'' as exercicio5,
						'' as exercicio6,
						'360' as prazo1,
						'' as prazo2,
						'' as prazo3,
						'' as prazo4,
						'' as prazo5,
						'10000' as real,
						'300300' as evento,
						'' as esfera,
						pro.proid
						/*
						tcp.tcpid as n_do_processo,
						tcp.tcpdscobjetoidentificacao as assunto,
						'??' as campus,
						ung.ungcod as ug,
						gescod as gestao,
						a.acacod as acao,
						--varios
						'300300' as evento,
						'1' as esfera,
						ptr.ptres as ptres,
						'??' as fonte,
						pli.plicod as pi,
						substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as nd,
						pro.provalor as valor_nd
						*/
					FROM
						monitora.termocooperacao tcp 
					INNER JOIN monitora.previsaoorcamentaria 	pro ON pro.tcpid = tcp.tcpid
					LEFT  JOIN monitora.ptres 			  p ON   p.ptrid = pro.ptrid
					LEFT  JOIN monitora.acao 			  a ON   a.acaid =   p.acaid
					LEFT  JOIN monitora.pi_planointerno		pli ON pli.pliid = pro.pliid
					LEFT  JOIN monitora.pi_planointernoptres 	pts ON pts.pliid = pli.pliid
					LEFT  JOIN monitora.ptres			ptr ON ptr.ptrid = pts.ptrid
					LEFT  JOIN public.naturezadespesa 		ndp ON ndp.ndpid = pro.ndpid
					LEFT  JOIN public.unidadegestora 		unp ON unp.ungcod = tcp.ungcodproponente
					LEFT  JOIN public.unidadegestora 		unc ON unc.ungcod = tcp.ungcodconcedente
					WHERE 
						tcp.tcpid = $tcpid";
			//dbg($sql);
			$comps = $db->carregar( $sql );
		}

		if($comps){
			
			foreach( $comps  as $y => $comp ){
				
				$n = $y+1;
				
				$dados[$k]['evento_'.$n] = $comp['evento'];
				$dados[$k]['esfera_'.$n] = $comp['esfera'];
				$dados[$k]['ptres_'.$n] = $comp['evento'];
				$dados[$k]['fonte_'.$n] = $comp['evento'];
				$dados[$k]['pi_'.$n] = $comp['evento'];
				$dados[$k]['nd_'.$n] = $comp['evento'];
				$dados[$k]['valor_nd_'.$n] = $comp['evento'];
				
				$cabecalho[] = 'Evento - '.$n;
				$cabecalho[] = 'Esfera - '.$n;
				$cabecalho[] = 'PTRES - '.$n;
				$cabecalho[] = 'Fonte - '.$n;
				$cabecalho[] = 'Plano Interno - '.$n;
				$cabecalho[] = 'Nat. da Despesa - '.$n;
				$cabecalho[] = 'Valor - '.$n;
			}
		
		}
		
	}
	
}	



// mostra relatório em XLS
if ( $_REQUEST['tipo'] == 'xls' )
{
	header( 'Content-type: application/xls' );
	header( 'Content-Disposition: attachment; filename="planilha_simec.xls"' );
	//$db->sql_to_excel( $dados, 'relorc', '', $tipo_campo );
	
	/*TESTE FIXO XLS*/
	$cabecalho = Array(	'UG PROPONENTE','GESTÃO PROPONENTE',
							   	'UG CONCEDENTE','GESTÃO CONCEDENTE',
							   	'NÚMERO ORIGINAL','TIPO DE CADASTRO',
							   	'RESPONSÁVEL 1','NÚMERO DO PROCESSO',
							   	'UG RECEBEDORA', 'GESTÃO RECEBEDORA',
							   	'RESPONSÁVEL 2','TÍTULO',
							   	'INÍCIO DA VIGÊNCIA','FIM DA VIGÊNCIA',
							   	'VALOR FIRMADO','VALOR TOTAL',
							   	'DESCRIÇÃO','JUSTIFICATIVA',
							   	'EXERCÍCIO 1','EXERCÍCIO 2',
							  	'PRAZO 1','PRAZO 2',
								'PRAZO 3','PRAZO 4',
								'PRAZO 5','(%) REAL',
								'Evento-1','Esfera-1',
								'PTRES-1','Fonte-1',
								'PI-1','Natureza Despesa-1',
								'Valor-1','N° de Registro');
								
			
	$dados = Array(Array('154049','15266',
				'150011 ','00001',
				'3','6',
				'34691723153','3',
				'154049','15266',
				'02011171857','PROJETO DA RESIDÊNCIA MULTIPROFISSIONAL EM SAÚDE DA FAMÍLIA E COMUNIDA',
				'26/03/2013','26/01/2014',
				'286.178,40','28617840',
				'Promover o desenvolvimento de atributos profissionais que possibilitem aos residentes formados pelo programa, o exercício profissional com excelência nas áreas de cuidado integral à saúde das pessoas e de comunidades e na gestão e organização do trabalho, visando à melhoria da saúde e da qualidade de vida.',
				'A parceria entre Universidade/Serviços estabelece uma articulação sistematizada e reflexiva entre o mundo do trabalho, da aprendizagem e a sociedade. Pressupõe que o Sistema de Saúde passe a operar com a lógica da formação e da educação permanente dos profissionais de saúde, em toda a rede de cuidados progressivos. As contrapartidas inter-instituic',
				'2013','2014',
				'360','',
				'','',
				'','10000',
				'300300','',
				'043253','',
				'FSS24O06MTS','339048',	
				'28617840','1'));

	$db->sql_to_excel( $dados, 'relorc', $cabecalho, '' );
	/*FIM TESTE FIXO XLS*/
	
	exit();
}
// FIM mostra relatório em XLS

function cabecalhoBrasao()
{
	global $db;
	global $consulta;
	?>
	<table width="100%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
		<tr bgcolor="#ffffff">
			<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>
			<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">
				<?php echo NOME_SISTEMA; ?><br/>
				Acompanhamento de Demandas<br/>
			</td>
			<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">
				Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
				Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?>
			</td>
		</tr>
		<tr>
			<td colspan="2" align="center" valign="top" style="padding:0 0 5px 0;">
				<b><font style="font-size:14px;">GUIA DE TRAMITAÇÃO N° <?=str_pad($_REQUEST['grpid'], 6,'0',STR_PAD_LEFT) ?></font></b>
			</td>
		</tr>
	</table>
	<?
}

?>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">
		<meta http-equiv="Expires" content="-1">
		<title>Relatório de Indicadores</title>
		<style type="text/css">
			
			@media print {.notprint { display: none }}

			@media screen {
			.notscreen { display: none;  }
			.div_rolagem{ overflow-x: auto; overflow-y: auto; width:19.5cm;height:350px;}
			.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
			}

			*{margin:0; padding:0; border:none; font-size:8px;font-family:Arial;}
			.alignRight{text-align:right !important;}
			.alignCenter{ text-align:center !important;}
			.alignLeft{text-align:left !important;}
			.bold{font-weight:bold !important;}
			.italic{font-style:italic !important;}
			.noPadding{padding:0;}
			.titulo{width:52px;}
			.tituloagrup{font-size:9px;}
			.titulolinha{font-size:9px;}
			
			#tabelaTitulos tr td, #tabelaTitulos tr th{border:2px solid black;border-left:none; border-right:none;}
			#orgao{margin:3px 0 0 0;}
			#orgao tr td{border:1px solid black;border-left:none;border-right:none;font-size:11px;}
			
			div.filtro { page-break-after: always; text-align: center; }
			
			table{width:19cm;border-collapse:collapse;}
			th, td{font-weight:normal;padding:4px;vertical-align:top;}
			thead{display:table-header-group;}
			table, tr{page-break-inside:avoid;}
			a{text-decoration:none;color:#3030aa;}
			a:hover{text-decoration:underline;color:#aa3030;}
			span.topo { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.topo:hover { background-color: #d0d0d0; }
			
		</style>
	</head>
	<body>
		<table>
			<tr>
				<th class="noPadding" align="left">
					<? cabecalhoBrasao(); ?>
					<table id="tabelaTitulos" align="left">
						<tr>
							<?
							if($cabecalho){	 
								foreach( $cabecalho as $coluna ){ 
								?>
								<td class="bold alignLeft"><?=$coluna;?></td>
								<?}
							}?>
						</tr>
						<?if($dados){
							foreach( $dados as $dado ){ 
							?> 
							<tr>
								<?foreach( $dado as $coluna ){ ?>
								<td><?=$coluna;?></td>
								<?}?>
							</tr>
							<?}
						}
						else{
							echo "<tr><td align='center'><font color=red><b>Não existem registros.</b></font></td></tr>";
						}
						?>
					</table>
				</th>
			</tr>
			<tr>
				<td class="noPadding" align="left">
					<div class="div_rolagem">
					</div>
				</td>
			</tr>
		</table>
	</body>
</html>