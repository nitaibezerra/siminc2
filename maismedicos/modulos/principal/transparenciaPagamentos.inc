<?php 

// define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../' ) );

// include_once BASE_PATH_SIMEC . "/global/config.inc";
// include_once APPRAIZ . "includes/classes_simec.inc";
// include_once APPRAIZ . "includes/funcoes.inc";
// include_once APPRAIZ . 'includes/classes/Modelo.class.inc';

// if(!$_SESSION['baselogin'] && in_array($_SERVER['HTTP_HOST'], array('simec-local','simec-d.mec.gov.br')))
// 	$_SESSION['baselogin'] = "simec_espelho_producao";

// if(!$_SESSION['usucpf'])
// 	$_SESSION['usucpforigem'] = '';

$db = new cls_banco();

if($_REQUEST['gerarPdf']){
		
	echo '<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
		  <link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
		  <script>
				function gerarPdfConfirma(data)
				{
					document.location.href = "/maismedicos/transparenciaPagamentos.php?gerarPdf=true&confirmaPdf=true&data="+data;
				}
		  </script>';
		
	if($_GET['data']){		
		$arWhere[] = " dt_ini_periodo ilike '{$_GET['data']}%' AND det.dt_fim_periodo ilike '{$_GET['data']}%' ";
	}
	
	$cabecalho = "<center>
					<img src=\"http://simec.mec.gov.br/imagens/brasao.gif\" width=\"45\" height=\"45\" border=\"0\"><br/>			
					<h4>MINISTÉRIO DA EDUCAÇÃO<br/>				
					EBSERH - EMPRESA BRASILEIRA DE SERVIÇOS HOSPITALARES</h4>			
					<h3>Projeto Mais Médicos para o Brasil<br/>
					Pagamentos de Bolsas – Tutoria e Supervisão</h3>			
				  </center><br/>";
	
	echo $cabecalho;
	
	$sql = "select
					uni.uninome,
					substr(tut.tutcpf,1,3) || '.***.***-**' as tutcpf,
	   				tut.tutnome,
	   				case when tut.tuttipo = 'T'
	   					then 'Tutor'
	   					else 'Supervisor'
	   				end as funcao,
					to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'MM/YYYY') as mes_referencia,
					to_char(dt_envio_email, 'DD/MM/YYYY') as data_pagamento,
	   				case when tut.tuttipo = 'T'
	   					then '5000'
	   					else '4000'
	   				end as valor					
	   			from
	   				maismedicos.tutor tut
	   			inner join
	   				maismedicos.universidade uni ON uni.uniid = tut.uniid
	   			inner join
	   				maismedicos.remessadetalhe det ON det.tutid = tut.tutid and det.cs_ocorrencia = '0000'
	   			inner join
	   				maismedicos.remessacabecalho cab ON cab.rmcid = det.rmcid
				inner join
					maismedicos.autorizacaopagamento aut ON det.rmdid = aut.rmdid
	   			left join
	   				maismedicos.situacaoregistro sit ON sit.strcod = det.cs_ocorrencia
				where 
					aut.apgstatus = 'A'
				and
					det.cs_ocorrencia = '0000'
	   			".($arWhere ? ' and '.implode(' and ', $arWhere) : '')."
	   			order by
	   				tut.tutnome, to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'YYYYMM')";
	
// 		ver($sql, d);
		$rs = $db->carregar($sql);
		
		$html = $cabecalho;
		if($rs){			
			$html .= '<table cellpadding="3" cellspacing="1" width="100%"
							style="
								width: 100%;
								font-size: 10px;
								padding: 3px;								
								border-top: 2px solid #404040;
								border-bottom: 3px solid #dfdfdf;								
								border-collapse: collapse;">
						<thead>
						<tr style="background: #CCCCCC;">
							<th>Nome</th>
							<th>CPF</th>
							<th>Instituição Supervisora</th>
							<th>Função</th>
							<th>Mês de Referência</th>
							<th>Data de Pagamento</th>
							<th>Valor (R$)</th>
						</tr>
						</thead>';
			foreach($rs as $dados){
				$html .= '<tr>
						<td>'.$dados['tutnome'].'</td>
						<td>'.$dados['tutcpf'].'</td>
						<td>'.trataNomeInstituicao($dados['uninome']).'</td>
						<td>'.$dados['funcao'].'</td>
						<td>'.$dados['mes_referencia'].'</td>
						<td>'.$dados['data_pagamento'].'</td>
						<td>'.formata_valor($dados['valor']).'</td>
					  </tr>';
			}
			$html .= '</table>';
			$html .= '<div style="text-align:left;font-size:9px;">';
			$html .= '<p>Total:&nbsp;'.count($rs).' registros,&nbsp;';
			$html .= 'Gerado em:&nbsp;'.date('d/m/Y H:i:s').'</p>';
			$html .= '</div>';
		}

		$content = http_build_query(array('conteudoHtml' => utf8_encode($html)));
		$context = stream_context_create(array('http' => array('method' => 'POST', 'content' => $content)));
	
		$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context);
	
		header('Content-Type: application/pdf');
		header("Content-Disposition: attachment; filename=TRANSPARENCIA_PAGAMENTOS_MAIS_MEDICO_" . date('YmdHis').'.pdf');
		echo $contents;
		die;
		
}

?>
<html>
	<head>
	
		<title><?php echo NOME_SISTEMA; ?></title>
		
		<script type="text/javascript" src="/includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery-1.10.2.min.js"></script>
		
		<script type="text/javascript">

		function gerarPdf(data)
		{
			//alert(data);
			if(data){
				
				//document.location.href = "/maismedicos/transparenciaPagamentos.php?gerarPdf=true&data="+data;

				var url = "/maismedicos/transparenciaPagamentos.php?gerarPdf=true&data="+data;
				var popUp = window.open(url, 'popupGeraArquivo', 'height=600,width=800,scrollbars=yes,top=50,left=200');
				popUp.focus();
				
			}			
		}
		
		</script>
		
		<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
		<link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
		
		<style>
		
			body{
				background-image:url('../../imagens/degrade-fundo-preto.png');
				background-repeat:repeat-x;
				background-color:#DF981A;
				margin:0px;
				padding-top:0px;
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
                        	<?php echo SIGLA_SISTEMA; ?><br/>
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
            		<font color="white" size="+2">Projeto Mais Médicos para o Brasil<br/>
					Pagamentos de Bolsas – Tutoria e Supervisão</font>
            		<br/>
            		<b style="color:white">Selecione o mês para gerar o PDF</b>
            	</td>
            </tr>
        </table>
        
        <center>
		<div style="background: white;padding:10px;width:400px;">
		<?php
		
		// Informativo de Pagamento de bolsas de Tutores / Supervisores
		$sql = "select 
					periodo,
					sum(tutor) as tutor,
					sum(supervisor) as supervisor,
					sum(total) as total,
					ordem
				from (
					select
						case when ( sup.tutcpf in ('41088840434','06123635468','55365949404','34754806468','') and substr(det.dt_ini_periodo,1,6) in ('201407','201408','201409','201410','201411') ) then '06/2014' 
						else substr(dt_ini_periodo, 5, 2) || '/' || substr(dt_ini_periodo, 1, 4) end as periodo,								
						substr(dt_ini_periodo, 5, 2) as mes,
						substr(dt_ini_periodo, 1, 4) as ano,
						count(tut.tutid) as tutor,
						count(sup.tutid) as supervisor,
						sum(vl_credito) as total,
						1 as ordem
					from 
						maismedicos.autorizacaopagamento aut
					inner join 
						maismedicos.remessadetalhe det ON det.rmdid = aut.rmdid
					left join
						maismedicos.tutor tut ON tut.tutid = det.tutid and tut.tuttipo = 'T'
					left join
						maismedicos.tutor sup ON sup .tutid = det.tutid and sup .tuttipo = 'S'
					where 
						aut.apgstatus = 'A'
					and
						det.cs_ocorrencia = '0000'
					group by 
						 sup.tutcpf, det.dt_ini_periodo
					order by 
						substr(dt_ini_periodo, 1, 4), substr(dt_ini_periodo, 5, 2)
				) as tbls 
				group by periodo, ordem
				order by substr(periodo,4,4) desc, substr(periodo,1,2) desc";
		
		$arrDados = $db->carregar($sql);
		?>
		<table border="0" cellpadding="3" cellspacing="1" align="center"  
							style="
								width: 400px;
								font-size: 11px;
								padding: 3px;								
								border-top: 2px solid #404040;
								border-bottom: 3px solid #dfdfdf;								
								border-collapse: collapse;								
							">	
			<tr>
				<th>Mês de Referência</th>
				<!-- <th>Qtde. de Tutores</th>
				<th>Qtde. de Supervisores</th> -->
			</tr>
				<?php
				$total = 0;
				$total_tutor = 0;
				$total_supervisor = 0;
				?>
				<?php foreach($arrDados as $key => $dado): ?>
					<?php 
					$total += $dado['total'];
					$total_tutor += $dado['tutor'];
					$total_supervisor += $dado['supervisor']; 
					?>
					<?php $arData = explode('/', $dado['periodo']); ?>
					<?php $arMes = explode('/', $dado['periodo']); ?> 
					<tr <?php echo "onclick=\"gerarPdf('".$arData[1].$arData[0]."')\" class=\"link\" "; ?>  <?php echo ($key%2) ? 'class="zebrado"' : ''; ?> title="Abrir Mês de <?php echo ucfirst(mes_extenso($arMes[0])).'/'.$arMes[1] ?>" alt="Abrir Mês de <?php echo ucfirst(mes_extenso($arMes[0])).'/'.$arMes[1] ?>">
						<td><?php echo ucfirst(mes_extenso($arMes[0])).'/'.$arMes[1] ?></td>
						<!--  <td class="numero"><?php echo $dado['tutor'] ?></td>
						<td class="numero"><?php echo $dado['supervisor'] ?></td> -->
					</tr>
				<?php endforeach; ?>
			<!-- 
			<tr>
				<th class="bold" >Total</th>
				<th class="numero" align="right"><?php echo number_format($total_tutor,0,",",".") ?></th>
				<th class="numero" align="right"><?php echo number_format($total_supervisor,0,",",".") ?></th>
			</tr>
			 -->
		</table>
		</div>
		</center>
			
	</body>
</html>
<?php 

function trataNomeInstituicao($nome)
{
	switch ($nome){
		case 'CASA DE SAÚDE SANTA MARCELINA': 
			$nome = 'Casa de Saúde Santa Marcelina';
			break;
		case 'ESCOLA DE SAÚDE DA FAMÍLIA VISCONDE DE SABÓIA':
			$nome = 'Escola de Saúde da Família Visconde de Saboia';
			break;
		case 'Escola de Saúde Pública do Ceará':
			$nome = 'Escola de Saúde Pública do Ceará';
			break;
		case 'Fundação do ABC':
			$nome = 'Faculdade de Medicina do ABC';
			break;
		case 'Fundação do ABC (Santo André)':
			$nome = 'Faculdade de Medicina do ABC';
			break;
		case 'Fundação Universidade de Brasília':
			$nome = 'Universidade de Brasília';
			break;
		case 'FUNDAÇÃO UNIVERSIDADE DE PERNAMBUCO':
			$nome = 'Universidade Estadual de Pernambuco';
			break;
		case 'FUNDAÇÃO UNIVERSIDADE DE PERNAMBUCO (ESTADUAL)':
			$nome = 'Universidade Estadual de Pernambuco';
			break;
		case 'Fundação Universidade Federal de Mato Grosso':
			$nome = 'Universidade Federal de Mato Grosso';
			break;
		case 'Fundação Universidade Federal de Mato Grosso do Sul':
			$nome = 'Universidade Federal de Mato Grosso do Sul';
			break;
		case 'FUNDAÇÃO UNIVERSIDADE FEDERAL DE RONDÔNIA':
			$nome = 'Universidade de Federal de Rondônia';
			break;
		case 'Fundação Universidade Federal de Roraima':
			$nome = 'Universidade Federal de Roraima';
			break;
		case 'Fundação Universidade Federal de São Carlos':
			$nome = 'Universidade Federal de São Carlos';
			break;
		case 'Fundação Universidade Federal de São João del Rei':
			$nome = 'Universidade Federal de São João del Rei';
			break;
		case 'Fundação Universidade Federal de Sergipe':
			$nome = 'Universidade Federal de Sergipe';
			break;
		case 'Fundação Universidade Federal do Acre':
			$nome = 'Universidade Federal do Acre';
			break;
		case 'Fundação Universidade Federal do Amapá':
			$nome = 'Universidade Federal do Amapá';
			break;
		case 'Fundação Universidade Federal do Maranhão':
			$nome = 'Universidade Federal do Maranhão';
			break;
		case 'Fundação Universidade Federal do Piauí':
			$nome = 'Universidade Federal do Piauí';
			break;
		case 'Fundação Universidade Federal do Rio Grande':
			$nome = 'Universidade Federal do Rio Grande';
			break;
		case 'Fundação Universidade Federal do Tocantins':
			$nome = 'Universidade Federal do Tocantins';
			break;
		case 'Fundação Universidade Federal do Vale do São Francisco':
			$nome = 'Universidade Federal do Vale do São Francisco';
			break;
		case 'Hospital Nossa Senhora da Conceição S.A.':
			$nome = 'Hospital Nossa Senhora da Conceição';
			break;
		case 'INSTITUTO DE MEDICINA INTEGRAL PROFESSOR FERNANDO FIGUEIRA':
			$nome = 'Instituto de Medicina Integral Professor Fernando Figueira';
			break;
		case 'SECRETARIA DE ESTADO DA SAÚDE DA BAHIA':
			$nome = 'Secretaria de Estado da Saúde da Bahia';
			break;
		case 'SECRETARIA DE ESTADO DA SAÚDE DE ALAGOAS':
			$nome = 'Secretaria de Estado da Saúde de Alagoas';
			break;
		case 'SECRETARIA DE ESTADO DA SAÚDE DO RIO GRANDE DO SUL':
			$nome = 'Secretaria de Estado da Saúde do Rio Grande do Sul';
			break;
		case 'Secretaria Estadual de Saúde do Amazonas':
			$nome = 'Secretaria Estadual de Saúde do Amazonas';
			break;
		case 'SECRETARIA MUNICIPAL DE SAÚDE DE BELO HORIZONTE':
			$nome = 'Secretaria Municipal de Saúde de Belo Horizonte';
			break;
		case 'SECRETARIA MUNICIPAL DE SAÚDE DE FORTALEZA':
			$nome = 'Secretaria Municipal de Saúde de Fortaleza';
			break;
		case 'Universidade da Integração Internacional da Lusofonia Afro-Brasileira':
			$nome = 'Universidade da Integração Internacional da Lusofonia Afro-Brasileira';
			break;
		case 'Universidade Estadual de Montes Claros':
			$nome = 'Universidade Estadual de Montes Claros';
			break;
		case 'Universidade Federal da Fronteira Sul':
			$nome = 'Universidade Federal da Fronteira Sul';
			break;
		case 'Universidade Federal de Alagoas':
			$nome = 'Universidade Federal de Alagoas';
			break;
		case 'Universidade Federal de Campina Grande':
			$nome = 'Universidade Federal de Campina Grande';
			break;
		case 'Universidade Federal de Goiás':
			$nome = 'Universidade Federal de Goiás';
			break;
		case 'Universidade Federal de Pernambuco':
			$nome = 'Universidade Federal de Pernambuco';
			break;
		case 'Universidade Federal de Santa Catarina':
			$nome = 'Universidade Federal de Santa Catarina';
			break;
		case 'Universidade Federal de Uberlândia':
			$nome = 'Universidade Federal de Uberlândia';
			break;
		case 'Universidade Federal do Cariri':
			$nome = 'Universidade Federal do Cariri';
			break;
		case 'Universidade Federal do Ceará':
			$nome = 'Universidade Federal do Ceará';
			break;
		case 'Universidade Federal do Espírito Santo':
			$nome = 'Universidade Federal do Espírito Santo';
			break;
		case 'Universidade Federal do Pará':
			$nome = 'Universidade Federal do Pará';
			break;
		case 'Universidade Federal do Paraná':
			$nome = 'Universidade Federal do Paraná';
			break;
		case 'Universidade Federal do Recôncavo da Bahia':
			$nome = 'Universidade Federal do Recôncavo da Bahia';
			break;
		case 'Universidade Federal do Rio de Janeiro':
			$nome = 'Universidade Federal do Rio de Janeiro';
			break;
		case 'Universidade Federal do Rio Grande do Norte':
			$nome = 'Universidade Federal do Rio Grande do Norte';
			break;
		case 'Universidade Federal do Rio Grande do Sul':
			$nome = 'Universidade Federal do Rio Grande do Sul';
			break;
		case 'Universidade Federal dos Vales do Jequitinhonha e Mucuri':
			$nome = 'Universidade Federal dos Vales do Jequitinhonha e Mucuri';
			break;
		case 'UNIVERSIDADE FEDERAL DE PELOTAS':
			$nome = 'Universidade Federal de Pelotas';
			break;
	}
	
	return $nome;
}

?>