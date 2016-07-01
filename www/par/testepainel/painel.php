<?php

/*
 * 
 * Colocar: 
 * 	Valor Pactuado, 
	QTD de termos PAR e PAC
	Filtro de Esfera
 */

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once "Grafico.php";
include '_funcoes_cockpit.php';

$db = new cls_banco();

$arrGrafico1 = array(
	array('descricao'=>'Mobiliario', 'valor'=>30),
	array('descricao'=>'Equipamentos', 'valor'=>10),
	array('descricao'=>'Inclusao e diversidade', 'valor'=>10),
	array('descricao'=>'Brasil Profissionalizado - Laboratorios', 'valor'=>20),
	array('descricao'=>'Instrumentos Musicais', 'valor'=>10),
	array('descricao'=>'Programa Caminho da Escola - Onibus Escolar', 'valor'=>20)
);

$arrGrafico2 = array(
	array('descricao'=>'PAR', 'valor'=>40),
	array('descricao'=>'PAC', 'valor'=>60)
);


?>
<html>
<head>

	<script language="javascript" type="text/javascript" src="../../library/jquery/jquery-1.11.1.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
    <link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
    <script language="javascript" src="../../includes/Highcharts-3.0.0/js/highcharts.js"></script>
    <script language="javascript" src="../../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>


   
	<style type="text/css">
		.quadros{
			background-image:url('../../library/jquery/jquery-ui-1.10.3/themes/dark-hive/images/ui-bg_loop_25_000000_21x21.png');
		}
		
		.subtitulo{
			font-weight: bold;
		}
	
	  #div-ciclos{height: 900px;}
	        #div-qtd{height: 550px;}
	        .fundo_titulo{
	        	background-image:url('fundo_enem.jpg');
	        	background-repeat: no-repeat;
	        	background-position:left;
	       		height: 150px;
	       		margin-left: 0;
	       		background-color: #FFFFFF;
	        };
	
	        .tabela_listagem {
	            background-color: #FFFFFF;
	            color: #000000;
	        }
	
	        .filtro_listagem{
	           width: 70%;
	        }
	        
	        .span_grupo{
	        	margin-right: 20px;
	        }
	 </style>
</head>

<body style=" background-image:url('fundo1.jpg'); background-repeat: repeat-x;" >
	<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
		<tr>
			<td class="titulo_pagina fundo_titulo" >
				<div>
					<img style="float:left" src="../../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico PAR</span></div>
				</div>
			</td>
		</tr>
		
	</table>
	<table border="0" align="center" width="99%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<tr>
		<td style="background-color: #1d1b1b; width: 10%;  " >
			<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
			<tr>
					<td>
					<div style="text-align: left !important;">
						<input type="button" id="buscar" value="Buscar" />
						<input type="button" id="limpar_dados" value="Limpar Filtros" />
						</div>
				
					</td>
			</tr>
			<tr>
					<td> 
						
						Programa: 
						<br/>
						<?php
						$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
						$db->monta_combo( "prgid", $sql, 'S', 'Programas', '', '','',189 );
						?>
					</td>
				</tr>
				<tr>
					<td> 
						
						Esfera
						<br/>
						<?php
						$sql = array(0 => array('codigo' => 1, 'descricao' => 'Estadual'), 1 => array('codigo' => 2, 'descricao' => 'Municipal'));
						
						 
						$db->monta_combo( "estuf", $sql, 'S', 'Esfera', '', '','',189 );
						?>
					</td>
				</tr>
				<tr>
					<td> 
						
						Estado
						<br/>
						<?php
						$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
						$db->monta_combo( "estuf", $sql, 'S', 'Unidades Federais', '', '','',189 );
						?>
					</td>
				</tr>
				<tr>
					<td> 
						
						Município: 
						<br/>
						<?php 
							echo campo_texto( 'municipio', 'N', 'S', '', 25, 200, '', ''); 
						?>
					</td>
				</tr>
				
				
			</table>
	</td>
	<td class="fundo_padrao "  align="middle" style=" background-color: #1d1b1b; " >
			<table border="0" align="center" width="100%" cellspacing="5" cellpadding="0" class="tabela_painel" >
				<tr>
					<td colspan="2"  >  
						
						<table border="0" align="left" width="100%" cellspacing="4" cellpadding="5" class="quadros tabela_painel" style="text-align: center; border: solid 3px #FFFFFF; margin-top: 3px;">
								<tr>
									<td class="subtitulo"  >Espírito Santo</td>
								</tr>
							</table>
						
					</td>
				</tr>
				<tr>
					<td>
							<table border="0" align="left" width="60%" cellspacing="4" cellpadding="5" class="quadros tabela_painel" style="text-align: center; border: solid 3px #FFFFFF; margin-top: 3px;">
								<tr>
									<td class="subtitulo"  >Valor Pactuado</td>
									<td class="subtitulo"  >Valor Empenhado</td>
									<td class="subtitulo" >Valor Repassado</td>
									<td class="subtitulo" >Saldo em Conta</td>
								</tr>
								<tr>
									<td>R$ 5.612.9149,57</td>
									<td>R$ 5.612.9149,57</td>
									<td>R$ 4.931.390,95</td>
									<td>R$ 2.931.390,95</td>
								</tr>
							</table>
					
							<table border="0" align="left" width="25%" cellspacing="4" cellpadding="5" class="quadros tabela_painel" style="text-align: center; border: solid 3px #FFFFFF; margin-left: 13px; margin-top: 3px; ">
								<tr>
									<td class="subtitulo" colspan="2"  >QTD de obras Financiadas</td>
								</tr>
								<tr>
									<td>PAR: 16</td>
									<td>PAC: 22</td>
								</tr>
							</table> 
							<table border="0" align="left" width="13%" cellspacing="4" cellpadding="5" class="quadros tabela_painel" style="text-align: center; border: solid 3px #FFFFFF; margin-left: 13px; margin-top: 3px; ">
								<tr style=" margin-top: 0px; margin-bottom: 0px;">
									<td colspan="2">QTD de Termos</td>
								</tr>
								<tr>
									<td class="subtitulo"  >PAR: 6</td>
									<td class="subtitulo"  >PAC: 2</td>
								</tr>
							</table> 
					</td>
				</tr>
				<tr>
					<td>
						<table border="0" align="left" width="50%" cellspacing="0" cellpadding="0" class="quadros tabela_painel" style="text-align: center; border: solid 3px #FFFFFF; margin-top: 3px; ">
								<tr>
									<td class="subtitulo">
									<div style="background-color: #FFFFFF; opacity:0.20; height: 20px; position: relative;"  ></div>
									<div style="position: relative; top:  -17px;">Repasse por Programas PAR </div>
									<div align="right" style="margin-right: 10px;"><input type="button" name="Detalhar" value="Detalhar"  ></div>
									</td>
								</tr>
								<tr>
									<td><?php  
									$grafico1 = new Grafico();
									$grafico1->width = '100%';
									
									$grafico1->gerarGrafico($arrGrafico1);
									 ?></td>
								</tr>
							</table> 
							
						<table border="0" align="left" width="49%" cellspacing="0" cellpadding="0" class="quadros tabela_painel" style="text-align: center; border: solid 3px #FFFFFF; margin-top: 3px; margin-left: 11px;  ">
								<tr>
									<td class="subtitulo">
									<div style="background-color: #FFFFFF; opacity:0.20; height: 20px; position: relative;"  ></div>
									<div style="position: relative; top: -17px;">Repasse de Obras </div>
									<div align="right" style="margin-right: 10px;" ><a href="http://simec-local/par/testepainel/tela2.php"><input type="button" name="Detalhar" value="Detalhar"  ></a></div>
									</td>
								</tr>
								<tr>
									<td>
										<?php  
									$grafico = new Grafico();
									$grafico->width = '100%';
									$grafico->gerarGrafico($arrGrafico2);
									 ?>
									
									</td>
								</tr>
							</table> 

					</td>
				</tr>
				
			</table>
	</td>
</tr>
</table>

</body>
</html>