<?php 

if($_POST){ 
	extract($_POST);
	foreach($_POST as $k => $v){
		$arParams[] = $k.'='.$v;
	}
	$stParams = implode('&',$arParams);
}

if($_GET) extract($_GET);

if($_SESSION['session_textoCaptcha'] != $txt_captcha) unset($_SESSION['maismedicos']['declaracao_pagamento']);

include_once APPRAIZ.'maismedicos/classes/Declaracao_Pagamento.class.inc';
$obDecPag = new Declaracao_Pagamento();

if($_REQUEST['tparquivo']){
	
	$html = $obDecPag->modeloDeclaracaoEbserh($_REQUEST);
	
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
	
		<title><?php echo NOME_SISTEMA; ?></title>
		
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery-1.10.2.min.js"></script>
		<script type="text/javascript" src="/includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="/includes/jquery-validate/localization/messages_ptbr.js"></script>
		<script type="text/javascript" src="/includes/jquery-validate/lib/jquery.metadata.js"></script>
		<script type="text/javascript" src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
		
		<script>
		 $(document).ready(function(){

		        var camposObrigatorios  = "[name=nucpf], [name=dtnascimento], [name=nubeneficio], [name=txt_captcha], [name=tpbolsista], [name=exercicio_declaracao]";
		        $(camposObrigatorios).addClass("required");

		        var camposData = "[name=dtnascimento]";
		        $(camposData).addClass("date");

		        var camposNumericos = "[name=nubeneficio]";
		        $(camposNumericos).addClass("number");

		        $("#formulario").validate();
		        
		 });

		 function gerarArquivo(tipo)
		 {
			var url = '/maismedicos/maismedicos.php?modulo=principal/declaracaoPagamentoMedico&acao=A&<?php echo $stParams; ?>&tparquivo='+tipo;
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
				background-color:#004500;
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
                    <div style="cursor:pointer;" onclick="window.location.href='/maismedicos/maismedicos.php?modulo=inicio&acao=C';">
                        <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
                        <div style="float:left" class="titulo_box" >
                        	<?php echo SIGLA_SISTEMA; ?><br/>
                        	<span class="subtitulo_box" >Mais Médicos</span>
                        </div>
                    </div>
                    	<div style="float:right;cursor:pointer;" onclick="javascript:history.go(-1)">		
                        	<img src="../imagens/icones/icons/voltar.png" style="vertical-align:middle;" title="Voltar" alt="Voltar" />		                        
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
<!--             <tr> -->
            	<td style="text-align:center;text-shadow:#000000 0px 4px 2px;">
<!--             		<br/><br/> -->
<!--             		<font color="white" size="+2">Declaração de Pagamento</font> -->
<!--             		<br/> -->
            		<?php if(empty($nucpf)): ?>
            			<b style="color:white">Preencha os campos abaixo</b>
            		<?php endif; ?>
<!--             	</td> -->
<!--             </tr> -->
        </table>
				
		<?php if($nucpf && $_SESSION['session_textoCaptcha'] == $txt_captcha): ?>
				
			<center>
			
				<div style="background:white;padding:10px;width:900px;">
				
				<?php 
				echo $obDecPag->modeloDeclaracaoEbserh($_REQUEST);
				?>
					
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
			
			$arPerfil = arrayPerfil();
			if(in_array(PERFIL_SUPERVISOR,$arPerfil) || in_array(PERFIL_TUTOR,$arPerfil)){
				$bolsista = $db->pegaLinha("select tutid, tutcpf, to_char(tutdatanascimento, 'DD/MM/YYYY') as tutdatanascimento
											,tuttipo 
											,(select nu_nib from maismedicos.remessadetalhe r where r.nu_cpf = t.tutcpf order by rmdid desc limit 1) as beneficio
											from maismedicos.tutor t where tutcpf='{$_SESSION['usucpf']}' order by tutid desc");
				$bolsista['tutcpf'] = formatar_cpf($bolsista['tutcpf']);
				echo "<script>
							$(function(){
								$('[name=nucpf]').val('{$bolsista['tutcpf']}').attr('readonly', true);
								$('[name=dtnascimento]').val('{$bolsista['tutdatanascimento']}').attr('readonly', true);
								$('[name=nubeneficio]').val('{$bolsista['beneficio']}').attr('readonly', true);
							});
					</script>";
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
						<td class="subtitulodireita" width="40%">Exercício:</td>
						<td><?php 
						$b=0;
						for($y=2013;$y<=date('Y');$y++){
							$anos_periodo[$b]['codigo'] = $y;
							$anos_periodo[$b]['descricao'] = $y;
							$b++;
						}
						
						$exercicio_declaracao = date('Y')-1;
						$db->monta_combo('exercicio_declaracao',$anos_periodo,'S','Ano',null,null,null,null,'N','exercicio_declaracao');
						?></td>
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