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

echo '<html>
  <head>
    <title>' , $titulo_modulo , '</title>
   <!-- <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
   -->
   	<!-- CSS -->
     <link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	  <link rel="stylesheet" type="text/css" href="../includes/listagem.css" />
    <style type="text/css">
    td.tab {
    }

    td.tab:hover {
        background-color: #ddd;
    }
    </style>
  </head>
  <body>
';

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
        $_SESSION['formacaoDisciplinaConfirmada'][$_REQUEST['muncod']][$tab['fodid']] = '<img src="../imagens/check_p.gif" style="margin-left: 2px; display: none;"/>';

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

    echo ' <form method="post" name="frmCadPlanejamentoEstrategico_' , $tab['fodid'] , '" id="frmCadPlanejamentoEstrategico_' , $tab['fodid'] , '" action="' , $_SERVER['REQUEST_URI'] , '" onsubmit="return validarFrmCadPlanejamentoEstrategico(this)">
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

            $input_est  = campo_texto('formacaolancamento[' . $grupo['fsgid'] . '_2]', 'N', 'S', '', '12 ', '9  ', '#.###.###', '', 'right', '', 0, 'id="formacaolancamento_' . $grupo['fsgid'] . '_2" onblur="MouseBlur(this);"');
            $input_mun  = campo_texto('formacaolancamento[' . $grupo['fsgid'] . '_3]', 'N', 'S', '', '12 ', '9  ', '#.###.###', '', 'right', '', 0, 'id="formacaolancamento_' . $grupo['fsgid'] . '_3" onblur="MouseBlur(this);"');

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

                $input_est  = campo_texto('formacaolancamento[' . $grupo['fsgid'] . '_2]', 'N', 'S', '', '12 ', '9  ', '#.###.###', '', 'right', '', 0, 'id="formacaolancamento_' . $grupo['fsgid'] . '_2" onblur="MouseBlur(this);"');
                $input_mun  = campo_texto('formacaolancamento[' . $grupo['fsgid'] . '_3]', 'N', 'S', '', '12 ', '9  ', '#.###.###', '', 'right', '', 0, 'id="formacaolancamento_' . $grupo['fsgid'] . '_3" onblur="MouseBlur(this);"');

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

