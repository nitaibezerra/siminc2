<?php 

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../' ) );

include_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';

if(!$_SESSION['baselogin'] && in_array($_SERVER['HTTP_HOST'], array('simec-local','simec-d.mec.gov.br')))
	$_SESSION['baselogin'] = "simec_espelho_producao";

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

$db = new cls_banco();

if($_POST){ 
	extract($_POST);
	foreach($_POST as $k => $v){
		$arParams[] = $k.'='.$v;
	}
	$stParams = implode('&',$arParams);
}

if($_GET) extract($_GET);

if($_SESSION['session_textoCaptcha'] != $txt_captcha) unset($_SESSION['maismedicos']['declaracao_pagamento']);

if($nucpf){
	$nucpftemp = str_replace(array('.','-'), '', $nucpf);
	$arWhere[] = " tut.tutcpf = '{$nucpftemp}'";
}

if($dtnascimento){
	$dtnascimentotemp = formata_data_sql($dtnascimento);
	$arWhere[] = " to_char(tut.tutdatanascimento, 'YYYY-MM-DD') = '{$dtnascimentotemp}' ";
}

if($nubeneficio){
	$arWhere[] = " det.nu_nib = '{$nubeneficio}' ";
}

if($tpbolsista){
	$arWhere[] = " tut.tuttipo = '{$tpbolsista}' ";
}

$sql = "select
   				tut.tutnome,
   				case when tut.tuttipo = 'T'
   					then 'Tutor'
   					else 'Supervisor'
   				end as funcao,
   				case when tut.tuttipo = 'T'
   					then '5000'
   					else '4000'
   				end as valor,
				to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'MM/YYYY') as periodo
   			from
   				maismedicos.tutor tut
   			inner join
   				maismedicos.universidade uni ON uni.uniid = tut.uniid
   			inner join
   				maismedicos.remessadetalhe det ON det.tutid = tut.tutid --and det.cs_ocorrencia = '0000'
   			inner join
   				maismedicos.remessacabecalho cab ON cab.rmcid = det.rmcid
   			inner join
   				maismedicos.folhapagamento fpg ON fpg.fpgid = cab.fpgid
			left join
				maismedicos.autorizacaopagamento apg on apg.rmdid = det.rmdid and apg.apgstatus = 'A'
   			left join
   				maismedicos.situacaoregistro sit ON sit.strcod = det.cs_ocorrencia
			where 
				tut.tutstatus = 'A'
   			".($arWhere ? ' and '.implode(' and ', $arWhere) : '')."
   			order by
   				tut.tutnome, to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'YYYYMM') asc";

$rsPagamentos = $db->carregar($sql);

if($nucpf && $dtnascimento && $nubeneficio){

	$sqlSol = "select
					t.tutcpf as cpf,
					t.tutnome as nome,
					to_char(t.tutdatanascimento,'DD/MM/YYYY') as nascimento,
					r.nu_nib as beneficio,
					case when t.tuttipo = 'S' then 'Supervisão'
					     when t.tuttipo = 'T' then 'Tutoria'
					else t.tuttipo end as tipo
				from maismedicos.tutor t
				join maismedicos.remessadetalhe r on t.tutcpf = r.nu_cpf
				where t.tutcpf = '{$nucpftemp}'
				and to_char(t.tutdatanascimento, 'YYYY-MM-DD') = '{$dtnascimentotemp}'
				and r.nu_nib = '{$nubeneficio}'";
	
	$solicitante = $db->pegaLinha($sqlSol);	
	
}

if($_REQUEST['tparquivo']){
	
	$html = '<center>
				<img src="http://simec.mec.gov.br/imagens/brasao.gif" width="45" height="45" border="0"><br/>			
				<h4>MINISTÉRIO DA EDUCAÇÃO<br/>				
				EBSERH - EMPRESA BRASILEIRA DE SERVIÇOS HOSPITALARES</h4>
				<h3>Declaração de Pagamento</h3>
			</center>';
	
	$html .= '<div style="background:white;padding:10px;width:100%;">';
	
	if($solicitante){
		
		$html .= '<div style="width: 100%; text-align: justify; text-justify: newspaper;">
					<div style="padding:10px;">
						&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;								
						Informa-se, para os devidos fins, que <b>'.$solicitante['nome'].'</b>, portador(a) 
						do CPF <b>'.formatar_cpf($solicitante['cpf']).'</b>, data de nascimento <b>'.$solicitante['nascimento'].'</b>, exerce a função de <b>'.$solicitante['tipo'].'</b> 
						no âmbito do Projeto Mais Médicos para o Brasil, e percebe benefício sob registro nº <b>'.$solicitante['beneficio'].'</b>, 
						na modalidade de bolsa, nos termos da Lei Nº 12.871/2013, conforme demonstrativo abaixo.
					</div>
				</div>';
		
		if($rsPagamentos){
			
			$html .= '<table border="0" cellpadding="3" cellspacing="1" align="center" width="100%" 
						style="
							width: 100%;
							font-size: 11px;
							padding: 3px;								
							border-top: 2px solid #404040;
							border-bottom: 3px solid #dfdfdf;								
							border-collapse: collapse;
						">							
						<thead>
							<tr style="background:#CCCCCC">
								<td style="text-align:left;"><b>Nome</b></td>
								<td style="text-align:center;"><b>Função</b></td>
								<td style="text-align:center;"><b>Valor (R$)</b></td>
								<td style="text-align:center;"><b>Mês</b></td>
							</tr>
						</thead>
						<tbody>';
			
				foreach($rsPagamentos as $indice => $pagamento){
					
					if (fmod($indice,2) == 0) $marcado = '' ; else $marcado='background:#F7F7F7;';
					
					$html .= '<tr style="'.$marcado.'">
								<td style="text-align:left;">'.$pagamento['tutnome'].'</td>
								<td style="text-align:center;">'.$pagamento['funcao'].'</td>
								<td style="color:blue;text-align:center;">'.formata_valor($pagamento['valor']).'</td>
								<td style="text-align:center;">'.$pagamento['periodo'].'</td>
							</tr>';
				}
				
				$html .= '</tbody>
					</table>';
				
		}else{
			
			$html .= '<p><font color="red">Não foram encontrados Registros.</font></p>';
		}
								
		$html .= '</div>';		
		
	}
	
	$html .= '<div style="text-align:left;font-size:9px;padding:15px;">';
	$html .= '<p>Total:&nbsp;'.count($rsPagamentos).' registros,&nbsp;';
	$html .= 'Gerado em:&nbsp;'.date('d/m/Y H:i:s').'</p>';
	$html .= '<p>Número de Autenticação:&nbsp;'.$_SESSION['maismedicos']['declaracao_pagamento']['cod_autenticacao'].'</p>';
	$html .= '</div>';
	
	ob_clean();
		
	$content = http_build_query(array('conteudoHtml' => utf8_encode($html)));
	$context = stream_context_create(array('http' => array('method' => 'POST', 'content' => $content)));
		
	$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);
		
	header('Content-Type: application/pdf');
	header("Content-Disposition: attachment; filename=DECLARACAO_PAGAMENTO_BOLSISTA_MAIS_MEDICO_" . date('YmdHis').'.pdf');
	echo $contents;
	exit;
	
}

?>
<html>
	<head>
	
		<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
		
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery-1.10.2.min.js"></script>
		<script type="text/javascript" src="/includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="/includes/jquery-validate/localization/messages_ptbr.js"></script>
		<script type="text/javascript" src="/includes/jquery-validate/lib/jquery.metadata.js"></script>
		<script type="text/javascript" src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
		
		<script>
		 $(document).ready(function(){

		        var camposObrigatorios  = "[name=nucpf], [name=dtnascimento], [name=nubeneficio], [name=txt_captcha], [name=tpbolsista]";
		        $(camposObrigatorios).addClass("required");

		        var camposData = "[name=dtnascimento]";
		        $(camposData).addClass("date");

		        var camposNumericos = "[name=nubeneficio]";
		        $(camposNumericos).addClass("number");

		        $("#formulario").validate();
		        
		 });

		 function gerarArquivo(tipo)
		 {
			var url = '/maismedicos/declaracaoPagamentoMedico.php?<?php echo $stParams; ?>&tparquivo='+tipo;
			var popUp = window.open(url, 'popupGeraArquivo', 'height=500,width=400,scrollbars=yes,top=50,left=200');
			popUp.focus();
			 
		 }
		</script>
		
		<link rel="stylesheet" type="text/css" href="/includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>		
		<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='/includes/listagem.css'/>
		<link rel="stylesheet" type="text/css" href="/includes/jquery-validate/css/validate.css" />
		<link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
		
		<style>
		
			body{
				background-image:url('../../imagens/degrade-fundo-preto.png');
				background-repeat:repeat-x;
				background-color:#DF981A;
				margin:0px;
				padding-top:0px;
			}
			th{
				background: #001944;
			}
			.fundo_titulo{
			    background-repeat:repeat-xt;
			    background-position:2px -50px;
			    font-weight:bold;
			    font-size:30px;
			    color:#FFFFFF;
			    text-shadow:#000000 0px 4px 2px;
			    background-image:url('images/bannerMaisMedicos.jpg')
			}
			
			.fundo_td{background-color:#0F6D39}
			.fundo_td:hover {background-color:#0D8845}
			.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
			.titulo_box{font-weight:bold;font-size:18px;color:#FFFFFF;margin-top:15px;text-shadow:#000000 0px 1px 2px}
			.subtitulo_box{font-weight:normal;font-size:12px;color:#FFFFFF}
			.fundo_td{text-align:left;vertical-align:top;}
			.tabela_painel{font-weight:bold;font-size:8px;color:#FFFFFF;font-family:fantasy}
			.lista_metas{float:left}
			#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
			.div_fotos{background-color:#7B68EE;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:350px;margin-bottom:2px}
			.div_fotos_padrao{background-color:#152D56;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:300px;margin-bottom:2px}
			.numero{text-align:right}
			.center{text-align:center}
			.titulo_box a{color:#FFFFFF;text-decoration:none;}
			.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
			.div_fotos_interno{margin-bottom:2px;width:98%}
			.bold{font-weight:bold}
			.link{cursor:pointer}			
			.numero{color: white; text-align: right;}		
			
			.fundo_padrao{background-color:#152D56}
			.fundo_padrao:hover {background-color:#1F3864}
			.fundo_azul{background-color:#2B86EE}
			.fundo_azul_padrao{background-color:#4F81BD}
			.fundo_verde{background-color:#0F6D39}
			.fundo_verde:hover{background-color:#32CD32}
			.fundo_laranja{background-color:#EE9200}
			.fundo_laranja:hover{background-color:#EBB513}
			.fundo_vermelho{background-color:#BB0000}
			.fundo_vermelho:hover{background-color:#DD0000}
			.fundo_roxo{background-color:#5333AD}
			.fundo_roxo:hover{background-color:#6A5ACD}
			.fundo_azul_escuro{background-color:#152D56}
			.fundo_azul_escuro:hover{background-color:#1F3864}
			.fundo_amarelo{background-color:#DAA520}
					
		</style>
		
	</head>
	<body>
			
		<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
            <tr>
                <td class="titulo_pagina" >
                    <div style="cursor:pointer;" onclick="window.location='/maismedicos/transparenciaPagamentos.php';">
                        <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
                        <div style="float:left" class="titulo_box" >
                        	SIMEC<br/>
                        	<span class="subtitulo_box" >Mais Médicos</span>
                        </div>
                    </div>
                    <div style="float:right;cursor:pointer;" onclick="window.location='/login.php';">		                    	  
                        <img src="../imagens/icones/icons/voltar.png" style="vertical-align:middle;" />		                        
                    </div>
                </td>
            </tr>
        </table>
        
		<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
            <tr>
            	<td class="fundo_titulo" style="text-align:center" >
            		<br/>Programa Mais Médicos<br/><br/>
            	</td>
            </tr>
            <tr>
            	<td style="text-align:center;text-shadow:#000000 0px 4px 2px;">
            		<br/><br/>
            		<font color="white" size="+2">Declaração de Pagamento</font>
            		<br/>
            		<?php if(empty($nucpf)): ?>
            			<b style="color:white">Preencha os campos abaixo</b>
            		<?php endif; ?>
            	</td>
            </tr>
        </table>
				
		<?php if($nucpf && $_SESSION['session_textoCaptcha'] == $txt_captcha): ?>
				
			<center>
			
				<div style="background:white;padding:10px;width:900px;">
				
				<?php if($solicitante): ?>
				
					<div style="width: 100%; text-align: justify; text-justify: newspaper;">
						<div style="padding:10px;">
							&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;								
							Informa-se, para os devidos fins, que <b><?php echo $solicitante['nome']; ?></b>, portador(a) 
							do CPF <b><?php echo formatar_cpf($solicitante['cpf']); ?></b>, data de nascimento <b><?php echo $solicitante['nascimento']; ?></b>, exerce a função de <b><?php echo $solicitante['tipo']; ?></b> 
							no âmbito do Projeto Mais Médicos para o Brasil, e percebe benefício sob registro nº <b><?php echo $solicitante['beneficio']; ?></b>, 
							na modalidade de bolsa, nos termos da Lei Nº 12.871/2013, conforme demonstrativo abaixo.
						</div>
					</div>
					
				<?php endif; ?>
				
				<?php 
				
				if(empty($_SESSION['maismedicos']['declaracao_pagamento']['id_autenticacao'])){
	
					$sqlInserLog = "insert into maismedicos.declaracao_pagamento 
										(nucpf,dtnascimento,tpbolsa,nubeneficio,nuip)
									values
										('{$nucpftemp}','{$dtnascimentotemp}','{$tpbolsista}','{$nubeneficio}','{$nuip}') returning decid;";
					
					$idLog = $db->pegaUm($sqlInserLog);
					$db->commit();
					
				}
				
				if($idLog) 
					$_SESSION['maismedicos']['declaracao_pagamento']['id_autenticacao'] = $idLog;
				else
					$idLog = $_SESSION['maismedicos']['declaracao_pagamento']['id_autenticacao'];
				
				$sqlAutenticacao = "select to_char(dtregistro,'YYYYMMDDHHIISS') || '-' || nubeneficio || '-' || lpad(decid::varchar, 8, '0') from maismedicos.declaracao_pagamento where decid = {$idLog}";
				$autenticacao = $db->pegaUm($sqlAutenticacao);
				
				$_SESSION['maismedicos']['declaracao_pagamento']['cod_autenticacao'] = $autenticacao;
				?>
				<?php if($rsPagamentos): ?>
					<table border="0" cellpadding="3" cellspacing="1" align="center" width="100%" 
						style="
							width: 100%;
							font-size: 11px;
							padding: 3px;								
							border-top: 2px solid #404040;
							border-bottom: 3px solid #dfdfdf;								
							border-collapse: collapse;
						">							
						<thead>
							<tr style="background:#CCCCCC">
								<td style="text-align:left;"><b>Nome</b></td>
								<td style="text-align:center;"><b>Função</b></td>
								<td style="text-align:center;"><b>Valor (R$)</b></td>
								<td style="text-align:center;"><b>Mês</b></td>
							</tr>
						</thead>
						<tbody>
						<?php foreach($rsPagamentos as $indice => $pagamento): ?>
							<tr>
								<td style="text-align:left;"><?php echo $pagamento['tutnome']; ?></td>
								<td style="text-align:center;"><?php echo $pagamento['funcao']; ?></td>
								<td style="color:blue;text-align:center;"><?php echo formata_valor($pagamento['valor']); ?></td>
								<td style="text-align:center;"><?php echo $pagamento['periodo']; ?></td>
							</tr>
						<?php endforeach; ?>
						</tbody>
					</table>
				<?php else: ?>
					<p><font color="red">Não foram encontrados Registros.</font></p>
				<?php endif; ?>
					
					<input type="button" value="Gerar PDF" onclick="gerarArquivo('pdf')" style="margin:10px;"/>
					<input type="button" value="Voltar" onclick="javascript:history.go(-1)" style="margin:10px;"/>
											
				</div>
				
			</center>
				
		<?php else: ?>
		
			<?php 
			if($_POST['txt_captcha']){
				if($_SESSION['session_textoCaptcha'] != $_POST['txt_captcha']){
					echo '<center><font color="red">A imagem não pode ser confirmada! Tente novamente.</font></center>';
				}
			} 
			?>
			
			<form name="formulario" id="formulario" method="post" action="">
			
				<table class="tabela" cellpadding="3" cellspacing="1" align="center" style="width:900px;">
					<tr>
						<td class="subtitulocentro" colspan="2">
							<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
							&nbsp;indica campo obrigatório.
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita">Tipo:</td>
						<td>
							<input type="radio" name="tpbolsista" value="T" <?php echo $tpbolsista=='T' ? 'checked' : '' ?>/>&nbsp;Tutor
							<input type="radio" name="tpbolsista" value="S" <?php echo $tpbolsista=='S' ? 'checked' : '' ?>/>&nbsp;Supervisor
							&nbsp;
							<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
						</td>
					</tr>
					<tr>
						<td class="subtitulodireita" width="40%">CPF:</td>
						<td><?php echo campo_texto('nucpf', 'S', 'S', '', '25', '15', '###.###.###-##', ''); ?></td>
					</tr>
					<tr>
						<td class="subtitulodireita">Data de Nascimento:</td>
						<td><?php echo campo_data2( 'dtnascimento', 'S', 'S', '', 'N','','' ); ?></td>
					</tr>
					<tr>
						<td class="subtitulodireita">Número do Benefício:</td>
						<td><?php echo campo_texto('nubeneficio', 'S', 'S', '', '25', '255', '', ''); ?></td>
					</tr>
					<tr>
						<td class="subtitulodireita">Confirme a Imagem:</td>
						<td>
							<img src="../captcha.php" width="113" height="49">
							<br/>
							<input type="text" name="txt_captcha" id="txt_captcha" maxlength="4" size="20"/>
							&nbsp;
							<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
						</td>
					</tr>
					<tr>
						<td colspan="2" class="subtitulocentro">
							<input type="hidden" name="nuip" value="<?php echo $_SERVER['REMOTE_ADDR']; ?>" />
							<input type="hidden" name="dtemissao" value="<?php echo date('YYYY-MM-DD'); ?>" />
							<input type="submit" value="Emitir" />
						</td>
					</tr>
				
				</table>
			
			</form>
			
		<?php endif; ?>
			
	</body>
</html>
<script>
$(function(){

	$('#rodape').width($(window).width());
	
});
</script>