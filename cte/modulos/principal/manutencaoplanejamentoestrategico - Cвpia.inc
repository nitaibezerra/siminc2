<?php
//
// $Id$
//



cte_verificaSessao();

if (!$_SESSION['formacaoDisciplinaConfirmada']) {
    $_SESSION['formacaoDisciplinaConfirmada'] = array();
}

if ($_REQUEST['fodid']) {
    //header('content-type: text/plain');
    //print_r($_REQUEST);

    $dados = $_REQUEST['formacaolancamento'];
    
	$input_est  = campo_texto('formacaolancamento[' . $grupo['fsgid'] . '_2]', 'N', 'S', '', '12 ', '9  ', '#.###.###', '', 'right', '', 0, 'id="formacaolancamento_' . $grupo['fsgid'] . '_2" onblur="MouseBlur(this);"');
	$input_mun  = campo_texto('formacaolancamento[' . $grupo['fsgid'] . '_3]', 'N', 'S', '', '12 ', '9  ', '#.###.###', '', 'right', '', 0, 'id="formacaolancamento_' . $grupo['fsgid'] . '_3" onblur="MouseBlur(this);"');

    $db->executar('DELETE FROM cte.formacaolancamento WHERE fodid = ' .$_REQUEST['fodid'].' AND tpiid = 2');

    $sql = 'INSERT INTO cte.formacaolancamento (
                tpiid,
                fsgid,
                renid,
                fodid,
                muncod,
                folvalor,
                datainclusao,
                usucpf) VALUES (2, %d, %d, %d, \'%s\', %s, now(), \'%s\')';

    foreach ($dados as $linha => $formacaolancamento) {
        $data  = explode('_', $linha);

        $fsgid = $data[0];
        $renid = $data[1];


        $sql2  = sprintf($sql,
                         $fsgid,
                         $renid,
                         (integer) $_REQUEST['fodid'],
                         (integer) $_REQUEST['muncod'],
                         (integer) (trim($formacaolancamento) == '' ? 0 : str_replace(",", ".", $formacaolancamento)), $_SESSION['usucpf']);

        $db->executar($sql2);

        //echo $formacaolancamento , "\n" , $sql2 , "\n\n";
    }

    $db->commit();

    $_SESSION['formacaoDisciplinaConfirmada'][$_REQUEST['muncod']][$_REQUEST['fodid']] = '<img src="/imagens/check_p.gif" style="margin-left: 2px;"/>';

    $alert = true;
    //die();
} else {
    $alert = false;
}
?>
<html>
<head>
    <title><?= $titulo_modulo ?></title>
  	<!--  <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
   <link rel="stylesheet" type="text/css" href="../includes/listagem.css" /> -->
    <link rel="stylesheet" type="text/css" href="../includes/principal.css" />

    <style type="text/css">
    td.tab {
    }

    td.tab:hover {
        background-color: #ddd;
    }
    </style>
    
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
</head>
<body>
	<div id="cadastro_form">
		<form action="" method="post">
				<fieldset class="campos_busca">				
					<label>
						Munic&iacute;pio:
						<select id="seleciona_municipio">
							<option value="0" id="mun_0"> -- Selecione -- </option>
						</select>
					</label>
					<input type="button" id="submit_busca" value="Buscar Dados" /> 
				</fieldset>
			</form>
			<span class="sigla_uf">
				UF
			</span>
			<span class="nome_municipio">
				Munic&iacute;pio
			</span>
			<ul id="menu_estados">
				<li id="tab_1"><a href="#" onclick="exibirTab(1)">Artes</a></li>       	 
				<li id="tab_2"><a href="#" onclick="exibirTab(2)">Biologia</a></li>       	 
				<li id="tab_3"><a href="#" onclick="exibirTab(3)">Ci&ecirc;ncias</a></li>       	 
				<li id="tab_4"><a href="#" onclick="exibirTab(4)">Educa&ccedil;&atilde;o F&iacute;sica</a></li>       	 
				<li id="tab_5"><a href="#" onclick="exibirTab(5)">Ens Fund Anos Iniciais</a></li>       	 
				<li id="tab_6"><a href="#" onclick="exibirTab(6)">Espanhol</a></li>       	 
				<li id="tab_7"><a href="#" onclick="exibirTab(7)">Filosofia</a></li>       	 
				<li id="tab_8"><a href="#" onclick="exibirTab(8)">F&iacute;sica</a></li>       	 
				<li id="tab_9"><a href="#" onclick="exibirTab(9)">Geografia</a></li>       	 
				<li id="tab_10"><a href="#" onclick="exibirTab(10)">Hist&oacute;ria</a></li>       	 
				<li id="tab_11"><a href="#" onclick="exibirTab(11)">Inform&aacute;tica</a></li>       	 
				<li id="tab_12"><a href="#" onclick="exibirTab(12)">Ingl&ecirc;s</a></li>       	 
				<li id="tab_13"><a href="#" onclick="exibirTab(13)">Matem&aacute;tica</a></li>       	 
				<li id="tab_14"><a href="#" onclick="exibirTab(14)">Outra L&iacute;ngua</a></li>       	 
				<li id="tab_15"><a href="#" onclick="exibirTab(15)">Portugu&ecirc;s</a></li>       	 
				<li id="tab_16"><a href="#" onclick="exibirTab(16)">Qu&iacute;mica</a></li>       	 
				<li id="tab_17"><a href="#" onclick="exibirTab(17)">Sociologia</a></li>       	 
			</ul>
			<form action="" method="post" id="cadastro_planejamento">
				<table cellpading="0" cellspacing="1">
					<thead>
						<tr>
							<td colspan="5" class="tit1">Confirma&ccedil;&atilde;o de Demanada de Professores</td>
						</tr>
					</thead>
					<tfoot>
						<tr>
							<td colspan="5" class="footer_submit">
								<input type="button" id="submit_button" class="plan_submit" value="Salvar" />
							</td>
						</tr>
					</tfoot>
					<tbody>
						<!-- Primeira Linha -->
						<tr>
							<td rowspan="2">&nbsp</td>
							<td colspan="2" class="tit2">Rede Municipal</td>
							<td colspan="2" class="tit2">Rede Estadual</td>
						</tr>
						<tr>
							<td class="tit3">Educacenso</td>
							<td class="tit3">A informar</td>
							<td class="tit3">Educacenso</td>
							<td class="tit3">A informar</td>				
						</tr>
						<tr class="linha">
							<td>Situa&ccedil;&otilde;es Perfeitas: Professores com Licenciatura na sua &Aacute;rea de Atua&ccedil;&atilde;o</td>
							<td> <input type="text" name="QtdProfApropriado_MUN" id="QtdProfApropriado_MUN" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="A_INF_QtdProfApropriado_MUN" name="A_INF_QtdProfApropriado_MUN" /></td>
							<td> <input type="text" name="QtdProfApropriado_EST" id="QtdProfApropriado_EST" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="A_INF_QtdProfApropriado_EST" name="A_INF_QtdProfApropriado_EST" /></td>
						</tr>
						<tr class="linha">
							<td>Professores sem Forma&ccedil;&atilde;o Superior</td>
							<td> <input type="text" name="QtdProfSemSup_MUN" id="QtdProfSemSup_MUN" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="A_INF_QtdProfSemSup_MUN" name="A_INF_QtdProfSemSup_MUN" /></td>
							<td> <input type="text" name="QtdProfSemSup_EST" id="QtdProfSemSup_EST" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="A_INF_QtdProfSemSup_EST" name="A_INF_QtdProfSemSup_EST" /></td>
						</tr>
						<tr class="linha">
							<td>Professores com Licenciatura mas n&atilde;o sendo na Disciplina</td>
							<td> <input type="text" name="QtdProfLecio_NA_MUN" id="QtdProfLecio_NA_MUN" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="A_INF_QtdProfLecio_NA_MUN" name="A_INF_QtdProfLecio_NA_MUN" /></td>
							<td> <input type="text" name="QtdProfLecio_NA_EST" id="QtdProfLecio_NA_EST" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="A_INF_QtdProfLecio_NA_EST" name="A_INF_QtdProfLecio_NA_EST" /></td>
						</tr>
						<tr class="linha">
							<td>Professores com N&iacute;vel Superior sem Licenciatura</td>
							<td> <input type="text" name="QtdProfSemLicenc_MUN" id="QtdProfSemLicenc_MUN" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="A_INF_QtdProfSemLicenc_MUN" name="A_INF_QtdProfSemLicenc_MUN" /></td>
							<td> <input type="text" name="QtdProfLecio_NA_MUN" id="QtdProfLecio_NA_MUN" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="QtdProfSemLicenc_EST" name="QtdProfSemLicenc_EST" /></td>
						</tr>
						<tr class="linha">
							<td>Demanda de Professores para Atender as Turmas sem Professor</td>
							<td> -- </td>
							<td><input type="text" class="input intermediario" name="A_INF_TURMA_SEM_PROF_MUN" id="A_INF_TURMA_SEM_PROF_MUN" /></td>
							<td> -- </td>
							<td><input type="text" class="input intermediario" name="A_INF_TURMA_SEM_PROF_EST" id="A_INF_TURMA_SEM_PROF_EST" /></td>
						</tr>
						<!-- Segunda Tabela -->
						<tr>
							<td colspan="5" class="tit1">Professores em Forma&ccedil;&atilde;o de Licenciatura na Disciplina</td>
						</tr>
						<tr>
							<td>&nbsp;</td>					
							<td class="tit3" colspan="2">Educacenso</td>
							<td class="tit3" colspan="2">A informar</td>				
						</tr>
						<tr class="linha">
							<td>Professores em Formação de Licenciatura na Disciplina </td>					
							<td class="tit3" colspan="2"><input type="text" class="input intermediario" name="A_INF_PROF_EM_FORMC_MUN" id="A_INF_PROF_EM_FORMC_MUN" /></td>
							<td class="tit3" colspan="2"><input type="text" class="input intermediario" name="A_INF_PROF_EM_FORMC_EST" id="A_INF_PROF_EM_FORMC_EST" /></td>				
						</tr>
						<!-- Terceira Tabela -->
						<tr>
							<td colspan="5" class="tit1">Resultado do Planejamento Estrat&eacute;gico do Estado, Reunindo a Secretaria de Educa&ccedil;&atilde;o, de Ci&ecirc;ncia e Tecnologia, UNDIME, Reitores das IFES e Estaduais e IFETs</td>
						</tr>
						<tr>
							<td rowspan="2">&nbsp</td>
							<td colspan="2" class="tit2">Rede Municipal</td>
							<td colspan="2" class="tit2">Rede Estadual</td>
						</tr>
						<tr>
							<td class="tit3">Educacenso</td>
							<td class="tit3">A informar</td>
							<td class="tit3">Educacenso</td>
							<td class="tit3">A informar</td>				
						</tr>
						<tr class="linha">
							<td>Previs&atilde;o da necessidade de forma&ccedil;&atilde;o de Professores</td>
							<td> <input type="text" name="TOTAL_MUN" id="TOTAL_MUN" class="wBord" />  </td>
							<td><input type="text" class="input intermediario" id="A_INF_TOTAL_MUN" name="A_INF_TOTAL_MUN" /></td>
							<td> <input type="text" name="TOTAL_EST" id="TOTAL_EST" class="wBord" />  </td>
							<td><input type="text" class="input intermediario" id="A_INF_TOTAL_EST" name="A_INF_TOTAL_EST" /></td>
						</tr>
						<tr class="linha">
							<td>Estimativa de Aposentadorias</td>
							<td><input type="text" name="PREV_APOSENT_MUN" id="PREV_APOSENT_MUN" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" name="A_CALC_PREV_APOSENT_MUN" id="A_CALC_PREV_APOSENT_MUN" /></td>
							<td><input type="text" name="PREV_APOSENT_EST" id="PREV_APOSENT_EST" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="PREV_APOSENT_EST" name="PREV_APOSENT_EST" /></td>
						</tr>
						<tr class="linha">
							<td>Total de Previs&atilde;o para Demanda de Forma&ccedil;&atilde;o de Professores</td>
							<td><input type="text" name="Total_prev_par_MUN" id="Total_prev_par_MUN" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="A_INF_Total_prev_par_MUN" name="A_INF_Total_prev_par_MUN" /></td>
							<td><input type="text" name="Total_prev_par_EST" id="Total_prev_par_EST" class="wBord" /> </td>
							<td><input type="text" class="input intermediario" id="A_INF_Total_prev_par_EST" name="A_INF_Total_prev_par_EST" /></td>
						</tr>
						<tr>
							<td>&nbsp</td>
							<td colspan="2" class="tit3">Educacenso</td>
							<td colspan="2" class="tit3">A informar</td>
						</tr>
						<tr class="linha">
							<td>Oferta Presencial das IES P&uacute;blicas Estaduais</td>					
							<td class="tit3" colspan="2"><input type="text" name="Oferta_IES_Est_Demanda_MUN" id="Oferta_IES_Est_Demanda_MUN" class="wBord" /> </td>
							<td class="tit3" colspan="2"><input type="text" class="input intermediario" name="Oferta_IES_Est_Demanda_EST" id="Oferta_IES_Est_Demanda_EST" /></td>				
						</tr>
						<tr class="linha">
							<td>Oferta Atrav&eacute;s de Cursos Presenciais das IFES</td>					
							<td class="tit3" colspan="2"><input type="text" name="Oferta_IFES_MUN" id="Oferta_IFES_MUN" class="wBord" /> </td>
							<td class="tit3" colspan="2"><input type="text" class="input intermediario" name="Oferta_IFES_EST" id="Oferta_IFES_EST"></td>				
						</tr>
						<tr class="linha">
							<td>Oferta Atrav&eacute;s de Cursos Presenciais dos IFETs</td>					
							<td class="tit3" colspan="2"><input type="text" name="Oferta_IFET_MUN" id="Oferta_IFET_MUN" class="wBord" /> </td>
							<td class="tit3" colspan="2"><input type="text" class="input intermediario" id="Oferta_IFET_EST" name="Oferta_IFET_EST"></td>				
						</tr>
						<tr class="linha">
							<td>Oferta Atrav&eacute;s da UAB</td>					
							<td class="tit3" colspan="2"><input type="text" name="Oferta_UAB_MUN" id="Oferta_UAB_MUN" class="wBord" /></td>
							<td class="tit3" colspan="2"><input type="text" class="input intermediario" id="Oferta_UAB_EST" name="Oferta_UAB_EST" /></td>				
						</tr>
					</tbody>
				</table>
			</form>
		<div>
<?php
if ($alert)
    alert('Dados salvos com sucesso.');

monta_titulo($titulo_modulo, null);//, '<img style="margin-right: 2px;" src="/imagens/atencao.png" align="middle" />Disciplinas com pendências&nbsp;&nbsp;&nbsp;<img style="margin-right: 2px;" src="/imagens/check_p.gif" align="middle" />Itens ok');
?>

    <table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="abasSuperiores" width="95%">
      <tr>
<?php


//                                                                          */
$sql = '
SELECT
    fd.fodid,
    fd.foddsc
FROM
    cte.formacaodisciplina fd
WHERE
    fodstatus = \'A\'
ORDER BY
    fd.foddsc ASC';

$disciplina = $db->carregar($sql);
$tabAtiva   = array_key_exists('fodid', $_REQUEST) ? $_REQUEST['fodid'] : null;

while (list($cont, $tab) = each($disciplina)) {
    if ($cont == 0 && $tabAtiva === null)
        $tabAtiva = $tab['fodid'];

    if (!$_SESSION['formacaoDisciplinaConfirmada'][$_REQUEST['muncod']][$tab['fodid']])
        $_SESSION['formacaoDisciplinaConfirmada'][$_REQUEST['muncod']][$tab['fodid']] = '<img src="/imagens/check_p.gif" style="margin-left: 2px; display: none;"/>';

    echo '        <td class="tab" id="tab_' , $tab['fodid'] , '" style="text-align: center;vertical-align: top">
          <a href="" onclick="return exibirTab(' , $tab['fodid'], ')">' , addslashes($tab['foddsc']) , '</a><br />' , $_SESSION['formacaoDisciplinaConfirmada'][$_REQUEST['muncod']][$tab['fodid']] , '
        </td>' , "\n";
}

//                                                                          */
?>
      </tr>
    </table>

<?php

//die();

$sql = 'SELECT
            fog.fogid,
            fog.fogordem,
            fog.fogdsc,
            fsg.fsgid,
            fsg.fsgordem,
            fsg.fsgdsc,
            fsg.indeducacenso
        FROM
            cte.formacaogrupo fog
        LEFT JOIN
            cte.formacaosubgrupo fsg ON fog.fogid = fsg.fogid
        WHERE
            fog.fogstatus = \'A\'
            AND
            fsg.fsgstatus = \'A\'
        GROUP BY
            fog.fogdsc,
            fog.fogordem,
            fsg.fsgdsc,
            fsg.fsgordem,
            fog.fogid,
            fsg.fsgid,
            fsg.indeducacenso
        ORDER BY
            fog.fogordem,
            fsg.fsgordem  ';

$fog = $db->carregar($sql);

$sqlDados = '
select
  fd.foddsc,
  fl.folvalor,
  fs.fsgdsc,
  fs.fsgid,
  fg.fogdsc,
  fg.fogid,
  re.rendsc,
  ti.tpidsc
from
	cte.formacaolancamento fl
left join
	cte.formacaodisciplina fd on fl.fodid = fd.fodid
left join
	cte.redeensino re on re.renid = fl.renid
left join
	cte.formacaosubgrupo fs on fl.fsgid = fs.fsgid
left join
	cte.formacaogrupo fg on fg.fogid = fs.fogid
left join
	cte.tipoinformacao ti on fl.tpiid = ti.tpiid
where
	muncod = \'' . $_REQUEST['muncod'] . '\'
	and
	fl.fodid = %d AND fl.fsgid = %d
order by
  ti.tpidsc asc,
  re.rendsc asc';


function atribuirValores($a)
{
    if (!is_array($a))
        $a = array();

    $dados = array('foddsc'   => null,
                   'folvalor' => '0',
                   'fsgdsc'   => null,
                   'fsgid'    => null,
                   'fogdsc'   => null,
                   'fogid'    => null,
                   'rendsc'   => null,
                   'tpidsc'   => null);

    return array_merge($dados, $a);
}



$tabela = false;
foreach ($disciplina as $i => $tab) {
    $total_informado_mun = 0;
    $total_informado_est = 0;

    echo '    <form method="post" name="frmCadPlanejamentoEstrategico_' , $tab['fodid'] , '" id="frmCadPlanejamentoEstrategico_' , $tab['fodid'] , '" action="' , $_SERVER['REQUEST_URI'] , '" onsubmit="return validarFrmCadPlanejamentoEstrategico(this)">
      <input type="hidden" name="fodid" value="' , $tab['fodid'] , '" />
      <table class="listagem" name="tableResult_' , $tab['fodid'] , '" width="95%" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3" align="center" id="table_' , $tab['fodid'] , '"><tbody>' , "\n";

    $ultimo = null;

    foreach ($fog as $grupo) {
        if ($ultimo === null || $ultimo != $grupo['fogid']) {
            echo '      <tr><td width="100%" colspan="5" style="background-color: #dcdcdc; font-weight: bold; text-align: center;">' , $grupo['fogdsc'] , '</td></tr>
                        <tr style="background-color: #dcdcdc;">
                            <td width="40%">&nbsp;</td>
                            <td style="text-align: center;font-weight: bold" colspan="2">Rede Estadual</td>
                            <td style="text-align: center;font-weight: bold" colspan="2">Rede Municipal</td>
                        </tr>

                        <tr style="background-color: #dcdcdc">
                            <td width="40%">&nbsp;</td>' , "\n";

            if ($grupo['indeducacenso'] == 'S') {
                echo '
                            <td align="center">Educacenso</td>
                            <td align="center">A informar</td>
                            <td align="center">Educacenso</td>
                            <td align="center">A informar</td>
                        </tr>' , "\n";
            } else {
                echo '
                            <td align="center" colspan="2">A informar</td>
                            <td align="center" colspan="2">A informar</td>
                        </tr>' , "\n";
            }

            $total_educacenso = 0;
            $total_informado  = 0;
        }


        $result = (array) $db->carregar(sprintf($sqlDados, $tab['fodid'], $grupo['fsgid']));
        //dump(sprintf($sqlDados, $tab['fodid'], $grupo['fsgid']), true);

        $res = array_map('atribuirValores', $result);

        if ($grupo['indeducacenso'] == 'S') {
            $total_informado_est += $res[2]['folvalor'];
            $total_informado_mun += $res[3]['folvalor'];

            $input_est  = 'formacaolancamento[' . $grupo['fsgid'] . '_2]';
            $input_mun  = 'formacaolancamento[' . $grupo['fsgid'] . '_3]';

            if (count($res) == 4) {
                $$input_est = $res[2]['folvalor'];
                $$input_mun = $res[3]['folvalor'];
            } else {
                $$input_est = $res[0]['folvalor'];
                $$input_mun = $res[1]['folvalor'];
            }

           

            echo '<tr bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';">
                    <td class="SubTituloDireita" align="right" width="40%">' , (strpos($grupo['fsgdsc'], 'Total de Previs') !== false ? '<strong>' . $grupo['fsgdsc'] . '</strong>' : $grupo['fsgdsc']) , '</td>
                    <td align="center">' , (trim($res[0]['folvalor']) != '' ? $res[0]['folvalor'] : '0') , '</td>
                    <td align="center">' , $input_est , '</td>
                    <td align="center">' , (trim($res[1]['folvalor']) != '' ? $res[1]['folvalor'] : '0') , '</td>
                    <td align="center">' , $input_mun , '</td>
                  </tr>' , "\n";
        } else {
            echo '<tr bgcolor="" onmouseout="this.bgColor=\'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
            if (strpos($grupo['fsgdsc'], 'Total de Previs') !== false) {
                echo '
                    <td class="SubTituloDireita" align="right" width="40%">' , (strpos($grupo['fsgdsc'], 'Total de Previs') !== false ? '<strong>' . $grupo['fsgdsc'] . '</strong>' : $grupo['fsgdsc']) , '</td>
                    <td align="center" colspan="2"><span id="totalizador_est">' , $total_informado_est , '</span></td>
                    <td align="center" colspan="2"><span id="totalizador_mun">' , $total_informado_mun , '</span></td>
                  </tr>' , "\n";
            } else {
                $input_est  = 'formacaolancamento[' . $grupo['fsgid'] . '_2]';
                $input_mun  = 'formacaolancamento[' . $grupo['fsgid'] . '_3]';

                echo "<!-- DEBUG\n" , print_r($res, true) , "\n-->\n";
                if (count($res) == 4) {
                    $$input_est = $res[2]['folvalor'];
                    $$input_mun = $res[3]['folvalor'];
                } else {
                    $$input_est = $res[0]['folvalor'];
                    $$input_mun = $res[1]['folvalor'];
                }

                $input_est  = campo_texto('formacaolancamento[' . $grupo['fsgid'] . '_2]', 'N', 'S', '', '12 ', '9  ', '', '', 'left', '', 0, 'id="formacaolancamento_' . $grupo['fsgid'] . '_2" onblur="MouseBlur(this);"');
                $input_mun  = campo_texto('formacaolancamento[' . $grupo['fsgid'] . '_3]', 'N', 'S', '', '12 ', '9  ', '', '', 'left', '', 0, 'id="formacaolancamento_' . $grupo['fsgid'] . '_3" onblur="MouseBlur(this);"');

                echo '
                    <td class="SubTituloDireita" align="right" width="40%">' , $grupo['fsgdsc'] , '</td>
                    <td align="center" colspan="2">' , $input_est , '</td>
                    <td align="center" colspan="2">' , $input_mun , '</td>
                  </tr>' , "\n";
            }
        }

        $ultimo = $grupo['fogid'];
    }

    echo '<tr style="background-color: #dcdcdc;">
            <td align="center" colspan="5"><input type="submit" value="Gravar" /></td>
          </tr>
          </tbody>
          </table>
        </form>' , "\n\n";

}
?>

    <script type="text/javascript">
    <!--
        var tabAtiva = null;

        /**
         * 
         */
        function exibirTab(tab)
        {
            if (tab == tabAtiva)
                return false;

            var form = document.getElementById('frmCadPlanejamentoEstrategico_' + tab);

            if (!validarFrmCadPlanejamentoEstrategico(form))
                return false;
            //                                                              */

            var tables = document.getElementsByTagName('table');

            for (var i = 0; i < tables.length; i++) {
                if (tables[i].getAttribute('name') != null &&
                    tables[i].getAttribute('name').indexOf('tableResult_') != -1)
                {
                    tables[i].style.display = 'none';
                }
                //                                                          */
            }

            var table = document.getElementById('table_' + tab);
            table.style.display = 'block';

            var abaSelecionada = document.getElementById('tab_' + tab);
            abaSelecionada.style.backgroundColor = '#ccc';

            if (tabAtiva != null) {
                var ultimaAba = document.getElementById('tab_' + tabAtiva);
                ultimaAba.style.backgroundColor = '#f5f5f5';
            }

            tabAtiva = tab;

            return false;
        }

        /**
         * 
         */
        function validarFrmCadPlanejamentoEstrategico(frm)
        {
            return true;
        }

        function totalizar(cont, el)
        {
            return false;

            /*!@
            var valor     = el.value.replace('.', '');
                valor     = valor.replace(',', '.');

            var container = document.getElementById(cont);
            var total     = container.innerHTML;

            container.innerHTML = total + valor;
            //                                                              */
        }

        exibirTab(<?php echo $tabAtiva; ?>);
      -->
    </script>
  </body>
</html>




