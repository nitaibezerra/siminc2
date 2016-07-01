<?php 
include_once '_funcoes_maismedicos.php';
$municipio = recuperarMunicipio();
$total_leitos = totalLeitos($_REQUEST['pmmid']);
$regiao_saude = recuperarRegiaoSaude();
$municipio_parceiro = recuperarMunicipioParceiro($_REQUEST['pmmid']);
$m = recuperaDadosPrefeitura();
$gestor_sus = recuperaGestorSusMunicipio();
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<html>
<head>
	<title>Mais Médicos - Termo de Parceria</title>
	<style>
            @media print { 
                .notprint {
                    display: none;
                }
                .div_rolagem {
                    display: none
                }
            }	
            .div_rolagem{ 
                overflow-x: auto; 
                overflow-y: auto; 
                height: 50px;
            }
            .notscreen {
                display: none;
            }				
            .bordaarredonda { 
                background:#FFFFFF; 
                color:#000; border: #000 1px solid; 
                padding: 10px; 
                -moz-border-radius:10px 10px;
                -webkit-border-radius:10px 10px;
                border-radius:10px 10px;
                width:95%;
                text-align:left;
            }		
            .quebra{ 
                page-break-after: always !important;
                height: 0px; 
                clear: both;
            }	
	</style>
</head>
<body>
<table border="0" width="85%" cellspacing="1" cellpadding="5" border="0" align="center">
 	<tr>
 		<td>
			<table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug">
				<tr bgcolor="#ffffff"> 	
					<td valign="top" align="center">
						<b>TERMO DE PARCERIA<br/>				
						<?php echo $municipio; ?></b> <br />
					</td>
				</tr>
			</table>						 		
 		</td>
 	</tr>
</table>
<br>
<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td style="font-size: 12px; font-family:arial;">
			<table align="center" border="0" cellspacing="1" cellpadding="3" width="85%">
				<tr>
					<td align="center">
						<p style="text-align: justify; font-size: 15px;">
				        Os municípios de <?php echo $municipio; ?> e de <?php echo $municipio_parceiro; ?> da Região de Saúde <?php echo $regiao_saude; ?> estabelecem entre si o presente Termo de Parceria, com o intuito de garantir o compromisso assumido 
				        junto ao Ministério da Educação, no âmbito do Edital nº 3, de 22 de outubro de 2013, que trata da pré-seleção de municípios para implantação de curso de graduação em medicina por instituição de educação superior privada.
				        </p>
					</td>
				</tr>	
				<tr>
					<td><p style="text-align: right;">&nbsp;</p></td>
				</tr>	
				<tr>
					<td align="center">
						<p style="text-align: justify; font-size: 15px;">
	        			Para consecução do objeto estabelecido no referido Edital, o município <?php echo $municipio; ?> se compromete a colocar à disposição da Instituição de Educação Superior privada vencedora de chamamento público a ser realizado pelo Ministério da Educação, <?php echo $total_leitos; ?> 
	        			(quantidade) leitos, (50, no mínimo), a fim de complementar o número de leitos exigidos pelo Edital nº 3, de 22 de outubro de 2013, para oferta como cenário de ensino do curso de medicina a ser potencialmente implantado no município de <?php echo $municipio; ?>.
	        			</p>
					</td>
				</tr>	
				<tr>
					<td><p style="text-align: right;">&nbsp;</p></td>
				</tr>					
				<tr>
					<td><p style="text-align: right;"><?php echo $municipio; ?> ,_____ de ______ 2013</p></td>
				</tr>
				<tr>
					<td><p style="text-align: right;">&nbsp;</p></td>
				</tr>		
				<tr>
					<td><p style="text-align: center;">______________________________________________</p></td>
				</tr>	
				<tr>
					<td><p style="font-family: Arial, verdana; font-size: 11px; text-align: center;"><?php echo $m['prefeito'] ? $m['prefeito'] : 'Não Informado';?> - Dirigente Municipal</p></td>
				</tr>	
				<tr>
					<td><p style="text-align: right;">&nbsp;</p></td>
				</tr>		
				<tr>
					<td><p style="text-align: right;">&nbsp;</p></td>
				</tr>	
				<tr>
					<td><p style="text-align: right;">&nbsp;</p></td>
				</tr>					
				<tr>
					<td><p style="text-align: center;">______________________________________________</p></td>
				</tr>		
				<tr>
					<td><p style="font-family: Arial, verdana; font-size: 11px; text-align: center;"><?php echo $gestor_sus ? $gestor_sus : 'Não Informado';?> - Gestor Local SUS</p></td>
				</tr>	
				<tr>
					<td><p style="text-align: right;">&nbsp;</p></td>
				</tr>										
			</table>
		</td>
	</tr>
</table>		
<br>
<table id="termo" width="95%" align="center" border="0" cellpadding="3" cellspacing="1">
	<tr>
		<td class="div_rolagem" style="text-align: center;">
			<input type="button" name="impressao" id="impressao" value="Imprimir" onclick="window.print();" />
			<input type="button" name="fechar" id="fechar" value="Fechar" onclick="window.close();" />
		</td>
	</tr>
</table>
</body>
</html>