<?php
//
// $Id$
//



cte_verificaSessao();

if (array_key_exists('mundescricao', $_REQUEST)) {
    $buscaMunicipio = ' AND UPPER(m.mundescricao) LIKE UPPER(\'%' . str_replace(' ', '%', $_REQUEST['mundescricao']) . '%\') ';
} else {
    $buscaMunicipio = '';
}

include APPRAIZ . 'includes/cabecalho.inc';

echo '<br />';

$db->cria_aba( $abacod_tela, $url, $parametros );

monta_titulo( $titulo_modulo, '<img style="margin-right: 2px;" src="/imagens/atencao.png" align="middle"/>Itens com pendências&nbsp;&nbsp;&nbsp;<img style="margin-right: 2px;" src="/imagens/check_p.gif" align="middle"/>Itens ok');

?>
    <script type="text/javascript">
    <!--
        function manterPlanejamentoEstrategico(muncod)
        {
            windowOpen('/cte/cte.php?modulo=principal/manutencaoplanejamentoestrategico&acao=C&muncod=' + muncod, 'manutencaoPlanejamentoEstrategico', 'height=650,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
            return false;
        }
      -->
    </script>

    <form method="post" name="frmBuscaMunicipio" id="frmBuscaMunicipio" action="<?php echo $_SERVER['REQUEST_URI']; ?>">
      <input type=hidden name="modulo" value="<?php echo $modulo ?>"/>
      <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td width="40%" align='right' class="SubTituloDireita">Munícipio:</td>
            <td><?php echo campo_texto('mundescricao', '', '', '', 30, 14, '','');?></td>
        </tr>
        <tr>
          <td colspan="2" class="SubTituloDireita" align="center">
            <input type='submit' class="botao" name='consultar' value='Consultar' />
          </td>
        </tr>
      </table>
    </form>
<?php

$total = $db->pegaUm('
SELECT (SELECT DISTINCT
            count(fsg.fsgid)
        FROM
            cte.formacaosubgrupo fsg
        INNER JOIN
            cte.formacaogrupo fog ON fsg.fogid = fog.fogid
        WHERE
            fog.fogstatus = \'A\'
            AND
            fsg.fsgstatus = \'A\'
        ) * 2 * (SELECT DISTINCT count(fod.fodid) FROM cte.formacaodisciplina fod WHERE fod.fodstatus = \'A\') AS total');


$sql = '
select
    \'<a href="/cte/cte.php?modulo=principal/planejamentoestrategico&acao=C" onclick="return manterPlanejamentoEstrategico(\' || m.muncod || \')" title=""><img style="border: none" src="/imagens/check_p.gif" title="Manter Planejamento Estrategico" /></a>\' as acao,
    m.muncod,
    CASE
        WHEN per.porcentagem = 100 THEN
            \'<img style="margin-right: 2px;" src="/imagens/check_p.gif" align="middle"/>&nbsp;<a href="/cte/cte.php?modulo=principal/planejamentoestrategico&acao=C" onclick="return manterPlanejamentoEstrategico(\' || m.muncod || \')" title="">\' || m.mundescricao || \'</a>\'
        ELSE
            \'<img style="margin-right: 2px;" src="/imagens/atencao.png" align="middle"/>&nbsp;<a href="/cte/cte.php?modulo=principal/planejamentoestrategico&acao=C" onclick="return manterPlanejamentoEstrategico(\' || m.muncod || \')" title="">\' || m.mundescricao || \'</a>\'
    END as descricao,
    \'<center><span style="color:cococo;font-size: 10px;">\' || coalesce(per.porcentagem, 0) || \'%</span>
    <div style="text-align: left; margin-left: 5px; padding: 1px 0 1px 0; height: 6px; max-height: 6px; width: 75px; border: 1px solid #888888; background-color:#dcffdc;" title="\' || coalesce(per.porcentagem, 0) || \'%">
    <div style="font-size:4px;width: \' || coalesce(per.porcentagem, 0)  || \'%; height: 6px; max-height: 6px; background-color:#339933;">
    </div></div></center>\' as porcentagem
from
    territorios.municipio m
left join
    (select muncod, (100 * count(folid)) / ' .$total. ' as porcentagem from cte.formacaolancamento where tpiid = 2 group by muncod) as per on per.muncod = m.muncod
left join
    (
        select
            iu.muncod,
            count(fs.fsgid) as total
        from
            cte.formacaogrupo fg 
        left join
            cte.formacaosubgrupo fs on fs.fogid = fg.fogid
        left join
            cte.formacaolancamento fl on fs.fsgid = fl.fsgid
        left join
            cte.instrumentounidade iu on iu.inuid = fl.inuid
        where
            iu.itrid = 1 or iu.itrid = 2
        group by
            iu.muncod
    ) as a ON a.muncod = m.muncod
where
    m.estuf = \'' . cte_pegarEstuf($_SESSION['inuid']) . '\'
    ' . $buscaMunicipio . '
group by
    m.muncod,
    m.mundescricao,
    per.porcentagem
order by
    m.mundescricao';

//dump($sql, true);
$db->monta_lista($sql, array('Ações', 'Código', 'Município', 'Status'), 25, 10, 'N', '', '');




//die();

/*!@
if ($_REQUEST['___a']) {
    if ($_REQUEST['___a'] == '1') {
        echo "<pre>\n" , print_r($_REQUEST, true);
        die();
    }

    $dados  = array(array('Professores Sem Formação Superior',
                          '00',
                          campo_texto('a1', 'N', 'S', ' ', '12 ', '9  ', '', '', 'left', '', 0, 'id="a1" onblur="MouseBlur(this);"'),
                          '00',
                          campo_texto('a2', 'N', 'S', ' ', '12 ', '9  ', '', '', 'left', '', 0, 'id="a2" onblur="MouseBlur(this);"')),
                    array('Professores Sem Licenciatura',
                          '00',
                          campo_texto('b1', 'N', 'S', ' ', '12 ', '9  ', '', '', 'left', '', 0, 'id="b1" onblur="MouseBlur(this);"'),
                          '00',
                          campo_texto('b2', 'N', 'S', ' ', '12 ', '9  ', '', '', 'left', '', 0, 'id="b2" onblur="MouseBlur(this);"')),
                    array('Turmas Sem Processo de Formação na Disciplina',
                          '00',
                          campo_texto('c1', 'N', 'S', ' ', '12 ', '9  ', '', '', 'left', '', 0, 'id="c1" onblur="MouseBlur(this);"'),
                          '00',
                          campo_texto('c2', 'N', 'S', ' ', '12 ', '9  ', '', '', 'left', '', 0, 'id="c2" onblur="MouseBlur(this);"')),
                    array('Professores Com Nível Superior Sem Licenciatura',
                          '00',
                          campo_texto('d1', 'N', 'S', ' ', '12 ', '9  ', '', '', 'left', '', 0, 'id="d1" onblur="MouseBlur(this);"'),
                          '00',
                          campo_texto('d2', 'N', 'S', ' ', '12 ', '9  ', '', '', 'left', '', 0, 'id="d2" onblur="MouseBlur(this);"')));

    echo '
      <table id="tblPlanejamentoEstrategico_' , $_REQUEST['muncod'] , '" class="tabela" align="center" border="0" cellpadding="5" cellspacing="1" style="width: 75%;">
        <tbody>
          <tr>
            <td style="font-weight: bold; background-color: #eee" align="center" colspan="5"><label>Confirmação de Demanda de Professores</label></td>
          </tr>

          <tr>
            <td style="background-color: #ccc; font-weight: bold;">&nbsp;</td>
            <td style="background-color: #ccc; font-weight: bold; text-align: center" colspan="2">Rede Municipal</td>
            <td style="background-color: #ccc; font-weight: bold; text-align: center" colspan="2">Rede Estadual</td>
          </tr>

          <tr>
            <td style="text-align:center; font-weight: bold">&nbsp;</td>
            <td style="text-align:center; font-weight: bold">Educacenso</td>
            <td style="text-align:center; font-weight: bold">Estadual</td>
            <td style="text-align:center; font-weight: bold">Educacenso</td>
            <td style="text-align:center; font-weight: bold">Municipal</td>
          </tr>' , "\n";

    foreach ($dados as $linha) {
        echo '
          <tr>
            <td style="font-weight: bold;" align="right" class="SubTituloDireita"><label for="input_xx_' , $_REQUEST['muncod'] , '">' , $linha[0] , '</label></td>
            <td style="text-align:center; background-color: #ccc;">' , $linha[1] , '</td>
            <td style="text-align:center;">'                         , $linha[2] , '</td>
            <td style="text-align:center; background-color: #ccc;">' , $linha[3] , '</td>
            <td style="text-align:center;">'                         , $linha[4] , '</td>
          </tr>' , "\n";
    }

    echo '
          <tr>
            <td style="font-weight: bold; background-color: #eee" align="center" colspan="5"><label>Totalizadores (Demanda)</label></td>
          </tr>

          <tr>
            <td style="font-weight: bold;" align="right" class="SubTituloDireita"><label>Totais</label></td>
            <td class="SubTituloDireita">Educacenso</td>
            <td style="text-align:center;">'                         , 00 , '</td>
            <td class="SubTituloDireita">Estadual + Municipal</td>
            <td style="text-align:center;">'                         , 00 , '</td>
          </tr>

          <tr>
            <td style="font-weight: bold;" align="right" class="SubTituloDireita"><label>Projeção de Crescimento da Rede</label></td>
            <td class="SubTituloDireita">Estadual</td>
            <td style="text-align:center;">'                         , $linha[2] , '</td>
            <td class="SubTituloDireita">Municipal</td>
            <td style="text-align:center;">'                         , $linha[4] , '</td>
          </tr>

          <tr>
            <td style="font-weight: bold; background-color: #eee" align="center" colspan="5"><label>Contratos Temporários</label></td>
          </tr>

          <tr>
            <td style="font-weight: bold;" align="right" class="SubTituloDireita"><label for="input_xx_' , $_REQUEST['muncod'] , '">Com Licenciatura</label></td>
            <td class="SubTituloDireita">Estadual</td>
            <td style="text-align:center;">'                         , $linha[2] , '</td>
            <td class="SubTituloDireita">Estadual</td>
            <td style="text-align:center;">'                         , $linha[4] , '</td>
          </tr>

          <tr>
            <td style="font-weight: bold;" align="right" class="SubTituloDireita"><label for="input_xx_' , $_REQUEST['muncod'] , '">Sem Licenciatura</label></td>
            <td class="SubTituloDireita">Estadual</td>
            <td style="text-align:center;">'                         , $linha[2] , '</td>
            <td class="SubTituloDireita">Municipal</td>
            <td style="text-align:center;">'                         , $linha[4] , '</td>
          </tr>

          <tr>
            <td style="font-weight: bold; background-color: #eee" align="center" colspan="5"><label>Projeção</label></td>
          </tr>

          <tr>
            <td style="font-weight: bold;" align="right" class="SubTituloDireita"><label>Professores em Formação</label></td>
            <td class="SubTituloDireita" colspan="4">' , 00 , '</td>
          </tr>

          <tr>
            <td style="font-weight: bold;" align="right" class="SubTituloDireita"><label>Projeção de Aposentadoria nos Próximos 8 Anos</label></td>
            <td class="SubTituloDireita" colspan="4">' , 00 , '</td>
          </tr>

          <tr>
            <td style="font-weight: bold;" align="right" class="SubTituloDireita"><label>Previsão da Necessidade na Formação de Professores</label></td>
            <td class="SubTituloDireita" colspan="4">' , 00 , '</td>
          </tr>

          <tr>
            <td style="font-weight: bold;" align="right" class="SubTituloDireita"><label>Demanda do PAR</label></td>
            <td class="SubTituloDireita">Estadual</td>
            <td style="text-align:center;">'                         , $linha[2] , '</td>
            <td class="SubTituloDireita">Municipal</td>
            <td style="text-align:center;">'                         , $linha[4] , '</td>
          </tr>

          <tr>
            <td style="font-weight: bold;" align="center" class="SubTituloDireita" colspan="5"><small>Todos os campos são obrigatórios</small></td>
          </tr>

        </tbody>
      </table>' , "\n";

    die();
}


$estuf = cte_pegarEstuf($_SESSION['inuid']);


$sql = "SELECT DISTINCT
            m.muncod,
            m.mundescricao
        FROM
            territorios.municipio m
        WHERE
            m.estuf = '$estuf'
        ORDER BY
            m.mundescricao ASC";

$cidades = $db->carregar($sql);



?><html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?php echo $_SESSION['sisdsc']; ?></title>
	<script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript" src="../includes/funcoes.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <script type="text/javascript">
    <!--
        var forms = new Array();
        var last  = null;

        function detalharPlanejamento(muncod)
        {
            var td  = $('containerDet_' + muncod);
            var tr  = $('containerFrm_' + muncod);
            var img = $('containerImg_' + muncod);

            if (last == null)
                last = muncod;

            if (tr.style.display != 'none') {
                tr.style.display = 'none';
                img.src          = '/imagens/mais.gif';
                last             = muncod;

                return false;
            } else {
                tr.style.display = !!document.all ? 'block' : 'table-row';
                img.src          = '/imagens/menos.gif';
            }

            if (typeof(forms[muncod]) != 'undefined' && forms[muncod]) {
                if (last != muncod) {
                    $('containerFrm_' + last).style.display = 'none';
                    $('containerImg_' + last).src           = '/imagens/mais.gif';
                    last                                    = muncod;
                }

                return false;
            } else {
                forms[muncod] = true;
            }

            if (last != muncod) {
                $('containerFrm_' + last).style.display = 'none';
                $('containerImg_' + last).src           = '/imagens/mais.gif';
                last                                    = muncod;
            }

            var req = new Ajax.Request('/cte/cte.php?modulo=principal/planejamentoestrategico&acao=C', {
                                       method: 'post',
                                       parameters: '&___a=b&opt=detalharPlanejamento&muncod=' + muncod,
                                       onComplete: function (res)
                                       {
                                           td.innerHTML = res.responseText;
                                       }
            });

            return false;
        }
      -->
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #f5f5f5; background: #fff url(../imagens/fundo.gif) repeat-y;">
    <?php monta_titulo( $titulo_modulo, '' ); ?>

    <form id="frmPlanejamentoEstrategico" action="<?php echo $_SERVER['REQUEST_URI']; ?>" method="post">
      <table id="tblPlanejamentoEstrategico" class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
        <tbody>
<?php
foreach ($cidades as $cidade) {
    echo '
          <tr bgcolor="#f9f9f9">
            <td style="padding-left: 0px;">
              <a href="' , $_SERVER['REQUEST_URI'] , '" onclick="return detalharPlanejamento(' , $cidade['muncod'] , ');" style="border: none;">
                <img src="../imagens/mais.gif" align="absmiddle" id="containerImg_' , $cidade['muncod'] , '" style="border: none;" />
                <strong>' , addslashes($cidade['mundescricao']) , '</strong>
              </a>
            </td>
          </tr>
          <tr id="containerFrm_' , $cidade['muncod'] , '" style="display: none;">
            <td id="containerDet_' , $cidade['muncod'] , '">&nbsp;</td>
          </tr>' , "\n";
}
?>
          <tr>
            <td><input type="submit" value="Gravar"/></td>
          </tr>
        </tbody>
      </table>

      <input type="hidden" name="___a" value="1" />

    </form>
  </body>
</html>


*/


