<?php
$entid = $_SESSION['pse']['entid'];
if (!$entid || !$_SESSION['pse']['sse']) {
    ?>
    <script>
        alert("Sua sessão expirou. Por favor, entre novamente!");
        window.close();
    </script>
    <?php
    exit;
}
$vpflcod = pegaPerfil($_SESSION['usucpf']);

if ($vpflcod == SUPER_USUARIO || $vpflcod == EDUCADOR_ESCOLA || $vpflcod == MEC) {
    $andDataLimite = " and a.avldtavaliacao > '2013-12-31'";
} else {
    $andDataLimite = " and a.avldtavaliacao > '2014-12-31'";
}


$dadosEscola = $db->pegaLinha("select distinct esc.idebp from pse.escolabasepse esc
							   inner join entidade.entidade ent on ent.entcodent = esc.idecodinep
                                                           left join pse.adesaoescola ade on esc.idebp = ade.idebp
							   where ent.entid = {$entid}
                                                           and comid = '2'");

if ($dadosEscola) {
    extract($dadosEscola);
}

if(!$idebp){
     ?>
    <script>
        alert("A escola não possui o número de idebp, número necessário para informar todas as ações!");
        window.close();
    </script>
    <?php
    exit;   
}


if ($_SESSION['pse']['sse'] == 'N') { // monitoramento de ações	
    //verifica se tem registro na tabela planoacao1
    $sql = "select count(entid) as total from pse.planoacaoc1 where entid = " . $entid;
    $totalPlanoC1 = $db->pegaUm($sql);

    //verifica se tem registro na tabela planoacao2
    $sql = "select count(entid) as total from pse.planoacaoc2 where entid = " . $entid;
    $totalPlanoC2 = $db->pegaUm($sql);
}

// monta cabeçalho
//include APPRAIZ . 'includes/cabecalho.inc';
echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

echo '
			<center>
				<table width="95%" border="0" cellpadding="0" cellspacing="0" class="notscreen1 debug"  style="border-bottom: 1px solid;">
					<tr bgcolor="#ffffff">
						<td valign="top" width="50" rowspan="2"><img src="../imagens/brasao.gif" width="45" height="45" border="0"></td>			
						<td nowrap align="left" valign="middle" height="1" style="padding:5px 0 0 0;">				
							SIMEC- Sistema Integrado do Ministério da Educação<br/>				
							Ministério da Educação - Ministério da Saúde<br />
						</td>
						<td align="right" valign="middle" height="1" style="padding:5px 0 0 0;">					
							Impresso por: <b>' . $_SESSION['usunome'] . '</b><br/>					
							Hora da Impressão:' . date('d/m/Y - H:i:s') . '<br />					
						</td>					
					</tr>				
				</table>
			</center>
		';

echo "<br>";


if ($_SESSION['pse']['sse'] == 'S') {
    $titulo = "Relatório das Ações da Semana Saúde na Escola";
} elseif ($_SESSION['pse']['sse'] == 'N') {
    $titulo = "Relatório das Ações do PSE na Escola";
} elseif ($_SESSION['pse']['sse'] == 'B') {
    $titulo = "Relatório das Ações do Brasil Carinhoso";
}
//$dsctitulo = 'Selecione uma ação abaixo.';

monta_titulo($titulo, '');

Cabecalho($entid, 1);
?>

<form id="formulario" name="formulario" action="" method="post">

    <table  class="Tabela" align="center"  cellpadding="2" cellspacing="1">

        <?php if ($_SESSION['pse']['sse'] != 'B') { ?>
            <tr>
                <td style="background: rgb(168, 168, 168);" colspan='3'><center>
                <span style="font-size: 12"><b>COMPONENTE II - PROMOÇÃO E PREVENÇÃO À SAÚDE</b></span>
            </center></td>
            </tr>
    <!--            <tr>
                <td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b>AÇÃO</b></span></td>
            </tr>-->
            <tr></tr>
            <tr>
                <td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b> Promoção da Segurança Alimentar e Promoção da Alimentação Saudável </b></span></td>
            </tr>
            <tr>
                <td> 
                    <?php
                    $sqlAcao1 = "SELECT
									'<center>'||n.niveldescricao||'<center>' as dsc, 
					            	'<center>'||a.avlqtpactuada||'<center>' as qtd,
					            	'<center>'||to_char(a.avldtavaliacao::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
					            	'<center>'||a.avlqtrealizada||'<center>' as qtd2,
					            	'<center>'||a.avlperctrealizao||' %<center>' as qtd3,
					            	'<center>' ||
									(CASE WHEN a.avlavalsseoumonit = 'S' THEN
											 '<FONT COLOR=BLUE>SIM</FONT>'
										  ELSE
											 '<FONT COLOR=RED>NÃO</FONT>'
									END) || '</center>' as opcao
								FROM pse.avaliaescola a
								INNER JOIN pse.niveldeensino n on n.idnivel=a.idnivel
								WHERE a.comid = 2
								and a.acaoid = 25
								and a.idebp = {$idebp}
                                                                {$andDataLimite}    
								order by 2";

                    $cabecalho = array("Nível", "Quantidade Pactuada", "Data da avaliação", "Quantidade Realizada", "Percentual Realizado", "Avaliação realizada na Semana Saúde na Escola");
                    $alinha = array("left", "center", "center", "center", "center", "center");
                    $tamanho = array("20%", "20%", "20%", "20%", "20%", "20%");

                    $db->monta_lista_simples($sqlAcao1, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
                    ?>
                </td>
            </tr>
            <tr>
                <td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b>  Promoção da Cultura de Paz e Direitos Humanos  </b></span></td>
            </tr>
            <tr>
                <td>
                    <?php
                    $sqlAcao2 = "SELECT
							'<center>'||n.niveldescricao||'<center>' as dsc, 
					            	'<center>'||a.avlqtpactuada||'<center>' as qtd,
					            	'<center>'||to_char(a.avldtavaliacao::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
					            	'<center>'||a.avlqtrealizada||'<center>' as qtd2,
					            	'<center>'||a.avlperctrealizao||' %<center>' as qtd3,
					            	'<center>' ||
									(CASE WHEN a.avlavalsseoumonit = 'S' THEN
											 '<FONT COLOR=BLUE>SIM</FONT>'
										  ELSE
											 '<FONT COLOR=RED>NÃO</FONT>'
									END) || '</center>' as opcao
								FROM pse.avaliaescola a
								INNER JOIN pse.niveldeensino n on n.idnivel=a.idnivel
								WHERE a.comid = 2
								and a.acaoid = 26
								and a.idebp = {$idebp}
                                                               {$andDataLimite}
								order by 2";

                    $cabecalho = array("Nível", "Quantidade Pactuada", "Data da avaliação", "Quantidade Realizada", "Percentual Realizado", "Avaliação realizada na Semana Saúde na Escola");
                    $alinha = array("left", "center", "center", "center", "center", "center");
                    $tamanho = array("20%", "20%", "20%", "20%", "20%", "20%");
                    $db->monta_lista_simples($sqlAcao2, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
                    ?>
                </td>
            </tr>
            <tr>
                <td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b>Saúde e Prevenção nas Escolas (SPE): Direito Sexual e Reprodutivo e Prevenção das DST/aids </b></span></td>
            </tr>
            <tr>
                <td>
                    <?php
                    $sqlAcao3 = "SELECT
							'<center>'||n.niveldescricao||'<center>' as dsc, 
					            	'<center>'||a.avlqtpactuada||'<center>' as qtd,
					            	'<center>'||to_char(a.avldtavaliacao::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
					            	'<center>'||a.avlqtrealizada||'<center>' as qtd2,
					            	'<center>'||a.avlperctrealizao||' %<center>' as qtd3,
					            	'<center>' ||
									(CASE WHEN a.avlavalsseoumonit = 'S' THEN
											 '<FONT COLOR=BLUE>SIM</FONT>'
										  ELSE
											 '<FONT COLOR=RED>NÃO</FONT>'
									END) || '</center>' as opcao
								FROM pse.avaliaescola a
								INNER JOIN pse.niveldeensino n on n.idnivel=a.idnivel
								WHERE a.comid = 2
								and a.acaoid = 28
								and a.idebp = {$idebp}
                                                               {$andDataLimite}
								order by 2";

                    $cabecalho = array("Nível", "Quantidade Pactuada", "Data da avaliação", "Quantidade Realizada", "Percentual Realizado", "Avaliação realizada na Semana Saúde na Escola");
                    $alinha = array("left", "center", "center", "center", "center", "center");
                    $tamanho = array("20%", "20%", "20%", "20%", "20%", "20%");
                    $db->monta_lista_simples($sqlAcao3, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
                    ?>
                </td>

            </tr>
            <tr>
                <td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b>      Saúde e Prevenção nas Escolas (SPE): Prevenção ao uso de Álcool, Tabaco, Crack e outras Drogas </b></span></td>
            </tr>
            <tr>
                <td>
                    <?php
                    $sqlAcao3 = "SELECT
							'<center>'||n.niveldescricao||'<center>' as dsc, 
					            	'<center>'||a.avlqtpactuada||'<center>' as qtd,
					            	'<center>'||to_char(a.avldtavaliacao::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
					            	'<center>'||a.avlqtrealizada||'<center>' as qtd2,
					            	'<center>'||a.avlperctrealizao||' %<center>' as qtd3,
					            	'<center>' ||
									(CASE WHEN a.avlavalsseoumonit = 'S' THEN
											 '<FONT COLOR=BLUE>SIM</FONT>'
										  ELSE
											 '<FONT COLOR=RED>NÃO</FONT>'
									END) || '</center>' as opcao
								FROM pse.avaliaescola a
								INNER JOIN pse.niveldeensino n on n.idnivel=a.idnivel
								WHERE a.comid = 2
								and a.acaoid = 29
								and a.idebp = {$idebp}
                                                               {$andDataLimite}
								order by 2";

                    $cabecalho = array("Nível", "Quantidade Pactuada", "Data da avaliação", "Quantidade Realizada", "Percentual Realizado", "Avaliação realizada na Semana Saúde na Escola");
                    $alinha = array("left", "center", "center", "center", "center", "center");
                    $tamanho = array("20%", "20%", "20%", "20%", "20%", "20%");
                    $db->monta_lista_simples($sqlAcao3, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
                    ?>
                </td>
            </tr>
            </tr>
            <tr>
                <td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b>   Promoção das Práticas Corporais, Atividade Física e Lazer nas Escolas </b></span></td>
            </tr>
            <tr>
                <td>
                    <?php
                    $sqlAcao3 = "SELECT
							'<center>'||n.niveldescricao||'<center>' as dsc, 
					            	'<center>'||a.avlqtpactuada||'<center>' as qtd,
					            	'<center>'||to_char(a.avldtavaliacao::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
					            	'<center>'||a.avlqtrealizada||'<center>' as qtd2,
					            	'<center>'||a.avlperctrealizao||' %<center>' as qtd3,
					            	'<center>' ||
									(CASE WHEN a.avlavalsseoumonit = 'S' THEN
											 '<FONT COLOR=BLUE>SIM</FONT>'
										  ELSE
											 '<FONT COLOR=RED>NÃO</FONT>'
									END) || '</center>' as opcao
								FROM pse.avaliaescola a
								INNER JOIN pse.niveldeensino n on n.idnivel=a.idnivel
								WHERE a.comid = 2
								and a.acaoid = 30
								and a.idebp = {$idebp}
                                                               {$andDataLimite}
								order by 2";

                    $cabecalho = array("Nível", "Quantidade Pactuada", "Data da avaliação", "Quantidade Realizada", "Percentual Realizado", "Avaliação realizada na Semana Saúde na Escola");
                    $alinha = array("left", "center", "center", "center", "center", "center");
                    $tamanho = array("20%", "20%", "20%", "20%", "20%", "20%");
                    $db->monta_lista_simples($sqlAcao3, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
                    ?>
                </td>
            </tr>  
            </tr>
            <tr>
                <td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b>   Promoção da Saúde Ambiental e Desenvolvimento Sustentável </b></span></td>
            </tr>
            <tr>
                <td>
                    <?php
                    $sqlAcao3 = "SELECT
							'<center>'||n.niveldescricao||'<center>' as dsc, 
					            	'<center>'||a.avlqtpactuada||'<center>' as qtd,
					            	'<center>'||to_char(a.avldtavaliacao::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
					            	'<center>'||a.avlqtrealizada||'<center>' as qtd2,
					            	'<center>'||a.avlperctrealizao||' %<center>' as qtd3,
					            	'<center>' ||
									(CASE WHEN a.avlavalsseoumonit = 'S' THEN
											 '<FONT COLOR=BLUE>SIM</FONT>'
										  ELSE
											 '<FONT COLOR=RED>NÃO</FONT>'
									END) || '</center>' as opcao
								FROM pse.avaliaescola a
								INNER JOIN pse.niveldeensino n on n.idnivel=a.idnivel
								WHERE a.comid = 2
								and a.acaoid = 31
								and a.idebp = {$idebp}
                                                               {$andDataLimite}
								order by 2";

                    $cabecalho = array("Nível", "Quantidade Pactuada", "Data da avaliação", "Quantidade Realizada", "Percentual Realizado", "Avaliação realizada na Semana Saúde na Escola");
                    $alinha = array("left", "center", "center", "center", "center", "center");
                    $tamanho = array("20%", "20%", "20%", "20%", "20%", "20%");
                    $db->monta_lista_simples($sqlAcao3, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
                    ?>
                </td>
            </tr>  
            <tr>
                <td align="center" style="background: rgb(218, 218, 218);"  colspan='3'><span style="font-size: 12"><b>Saúde e Prevenção nas Escolas (SPE): Formação de jovens multiplicadores para atuarem entre pares nas temáticas do direito sexual e reprodutivo e prevenção das DST/aids</b></span></td>
            </tr>
            <tr>
                <td>
                    <?php
                    $sqlAcao3 = "SELECT
							'<center>'||n.niveldescricao||'<center>' as dsc, 
					            	'<center>'||a.avlqtpactuada||'<center>' as qtd,
					            	'<center>'||to_char(a.avldtavaliacao::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
					            	'<center>'||a.avlqtrealizada||'<center>' as qtd2,
					            	'<center>'||a.avlperctrealizao||' %<center>' as qtd3,
					            	'<center>' ||
									(CASE WHEN a.avlavalsseoumonit = 'S' THEN
											 '<FONT COLOR=BLUE>SIM</FONT>'
										  ELSE
											 '<FONT COLOR=RED>NÃO</FONT>'
									END) || '</center>' as opcao
								FROM pse.avaliaescola a
								INNER JOIN pse.niveldeensino n on n.idnivel=a.idnivel
								WHERE a.comid = 2
								and a.acaoid = 35
								and a.idebp = {$idebp}
                                                               {$andDataLimite}
								order by 2";

                    $cabecalho = array("Nível", "Quantidade Pactuada", "Data da avaliação", "Quantidade Realizada", "Percentual Realizado", "Avaliação realizada na Semana Saúde na Escola");
                    $alinha = array("left", "center", "center", "center", "center", "center");
                    $tamanho = array("20%", "20%", "20%", "20%", "20%", "20%");
                    $db->monta_lista_simples($sqlAcao3, $cabecalho, 1000, 10, 'N', '', '', '', '', '', true);
                    ?>
                </td>
            </tr>
            <table>
            <?php } ?>
            <table align="center">
                <tr>
                    <td>
                        <input type="button" value="Imprimir" style="cursor: pointer, info{ display: none; }" onclick="self.print();">
                    </td>
                </tr>
            </table>


            </form>
