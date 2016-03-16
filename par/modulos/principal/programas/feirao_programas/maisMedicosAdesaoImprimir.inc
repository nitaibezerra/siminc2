<?php
include_once '_funcoes_maismedicos.php';
$m = recuperaDadosPrefeitura();
$gestor_sus = recuperaGestorSusMunicipio();
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

<html>
<head>
	<title>Mais Médicos - Termo de Adesão</title>
	<style>
		@media print {
                    .notprint {
                        display: none;
                    }
                    .div_rolagem {
                        display: none;
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
						<p align="center" style="margin-bottom: 0px;"><font face="Arial, sans-serif" size="3"><b>Termo de Adesão ao Programa Mais Médicos</b></font></p><br>
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
						<p align="justify" style="text-indent: 0.59in;"><font size="3">TERMO DE ADESÃO QUE ENTRE SI CELEBRAM A SECRETARIA DE
						REGULAÇÃO E SUPERVISÃO DA EDUCAÇÃO SUPERIOR DO MINISTÉRIO DA EDUCAÇÃO E O MUNICÍPIO DE <?php echo strtoupper($m['municipio']);?>, NO ESTADO DO <?php echo strtoupper($m['estado']);?>, PARA
						DISPONIBILIZAÇÃO DE SERVIÇOS, AÇÕES E PROGRAMAS DE SAÚDE NECESSÁRIOS À AUTORIZAÇÃO DE FUNCIONAMENTO DE CURSOS DE MEDICINA.
						</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">
						O MINISTÉRIO DA EDUCAÇÃO, CNPJ nº 00.394.445/0001-01, neste ato representado por JORGE RODRIGO ARAÚJO MESSIAS, Secretário de Regulação e Supervisão da Educação Superior, com endereço na Esplanada dos Ministérios, Bloco "L", 1º andar, sala 100 - CEP 70.047-900, Brasília (DF), e o MUNICÍPIO DE <?php echo strtoupper($m['municipio'])."-".strtoupper($m['estuf']);?>, <?php echo $m['endereco'];?>, neste ato representado por <?php echo $m['prefeito'];?> (Prefeito), nos termos da Medida Provisória nº 621, de 8 de julho de 2013, resolvem celebrar o presente Termo de Adesão para implantação e funcionamento de cursos de medicina,
						por instituição de educação superior privada, mediante as cláusulas e condições seguintes:
						</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">1. CLÁUSULA PRIMEIRA - DO OBJETO</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">1.1. O presente termo tem por objeto a adesão do Município de <?php echo $m['municipio']."-".$m['estuf'];?> ao Chamamento Público previsto no Edital SERES nº 03/2013 e definição de obrigações e responsabilidades do Município no oferecimento de estrutura de serviços, ações e programas de saúde necessários para implantação e funcionamento de curso de graduação em medicina a ser ofertado pela instituição de educação superior privada autorizada pelo Ministério da Educação.</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">2. CLÁUSULA SEGUNDA - DAS OBRIGAÇÕES DO MUNICÍPIO </font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">2.1. Para consecução do objeto estabelecido neste Termo de Adesão, o Município deverá atender aos seguintes critérios relativos à estrutura de equipamentos públicos e programas de saúde existentes e disponíveis em sua rede, previstos no Artigo 5º, da Portaria Normativa nº 13, de 2013, além de outros que podem ser estabelecidos pela SERES:</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						a) número de leitos disponíveis SUS por aluno, maior ou igual a 5 (cinco);</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						b) número de alunos por equipe de atenção básica menor ou igual a 3 (três);</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						c) existência de leitos de urgência e emergência ou Pronto Socorro;</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						d) comprometimento dos leitos SUS para utilização acadêmica;</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						e) existência de, pelo menos, 03 (três) Programas de Residência Médica nas especialidades prioritárias (Clínica Médica, Cirurgia, Ginecologia-Obstetrícia, Pediatria e Medicina da Família e Comunidade);</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						f) adesão pelo município ao Programa Nacional de Melhoria do Acesso e da Qualidade na Atenção Básica - PMAQ;</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						g) existência de Centro de Atenção Psicossocial - CAPS;</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						h) hospital de ensino ou unidade hospitalar com potencial para hospital de ensino, conforme legislação de regência; e</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						i) existência de hospital com mais de 100 (cem) leitos exclusivos para o curso.</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">3. CLÁUSULA TERCEIRA - DAS OBRIGAÇÕES DA SECRETARIA DE REGULAÇÃO E SUPERVISÃO DO MINISTÉRIO DA EDUCAÇÃO</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">3.1. Constituem obrigações da SERES:</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						a) selecionar os Municípios para implantação e funcionamento de cursos de medicina, por instituições de educação superior privadas;</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						b) celebrar Termo de Adesão com o município selecionado, acompanhar e monitorar a implantação do curso de medicina naquela localidade;</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						c) selecionar as instituições de educação superior privadas para oferta de cursos de graduação em medicina nos Municípios pré-selecionados; e</font></p>
						<p align="justify" style="margin-left: 0.98in;"><font size="3" style="font-size: 11pt">
						d) editar normas complementares necessárias ao cumprimento do estabelecido neste Termo de Adesão.</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">4. CLÁUSULA QUARTA - DA VIGÊNCIA</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">4.1. Os compromissos assumidos pelo Município no presente Termo de Adesão estão vinculados à regularidade da oferta do curso pela instituição de educação superior.</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">5. CLÁUSULA QUINTA - DA RESCISÃO</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">5.1. No caso de rescisão do presente Termo, cumpre ao Município informar à instituição de educação superior privada ofertante do curso e à Secretaria de Regulação e Supervisão da Educação Superior, com antecedência mínima
						de 90 (noventa) dias, a fim de preservar a continuidade da oferta do curso</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">6. CLÁUSULA SÉTIMA - DA PUBLICAÇÃO</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">6.1. O presente Termo de Adesão deverá ser publicado em extrato no Diário Oficial da União, a expensas do Ministério da Educação.</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">7. CLÁUSULA OITAVA - DAS ALTERAÇÕES</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">7.1. As eventuais alterações do presente Termo de Adesão serão realizadas por meio de termo aditivo acordado entre os partícipes.</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">8. CLÁUSULA NONA - DA SOLUÇÃO DE CONTROVÉRSIAS</font></p>
						<p align="justify" style="text-indent: 0.59in;"><font size="3">8.1. Eventual controvérsia surgida durante a execução do presente Termo de Adesão poderá ser dirimida administrativamente entre os partícipes ou, em seguida, perante a Câmara de Conciliação e Arbitragem da Administração Federal da Advocacia- Geral da União e, se inviável, posteriormente
						perante o foro da Justiça Federal - Seção Judiciária do Distrito Federal.</font></p>
						<p align="justify" style="margin-left: 0.59in;"><font size="3">E por estarem de pleno acordo, firmam este instrumento em 2 (duas) vias de igual teor e forma, para que produza seus jurídicos e legais efeitos.</font></p>
						<br><br><p align="right" style="margin-right: 0.98in;"><font size="3">Brasília-DF, ___ de ____________ de 2013.</font></p>
						<br><br><p align="right" style="margin-right: 0.98in;"><font size="3">____________________________________</font></p>
						<p align="right" style="margin-right: 0.98in;"><font size="3"><b>JORGE RODRIGO ARAÚJO MESSIAS</b></font></p>
						<p align="right" style="margin-right: 0.98in;"><font size="3">SECRETÁRIO DE REGULAÇÃO E SUPERVISÃO DA EDUCAÇÃO SUPERIOR</font></p>
						<br><br><p align="right" style="margin-right: 0.98in;"><font size="3">_____________________________________________________</font></p>
						<p align="right" style="margin-right: 0.98in;"><font size="3"><b><?php echo $m['prefeito']; ?></b></font></p>
						<p align="right" style="margin-right: 0.98in;"><font size="3">PREFEITO MUNICIPAL DE <?php echo strtoupper($m['municipio'])."-".strtoupper($m['estuf']);?></font></p>
						<br><br><p align="right" style="margin-right: 0.98in;"><font size="3">_____________________________________________________</font></p>
						<p align="right" style="margin-right: 0.98in;"><font size="3"><b><?php echo $gestor_sus ? strtoupper($gestor_sus) : 'Não Informado'; ?></b></font></p>
						<p align="right" style="margin-right: 0.98in;"><font size="3">GESTOR LOCAL DO SISTEMA ÚNICO DE SAÚDE</font></p>
					</td>
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