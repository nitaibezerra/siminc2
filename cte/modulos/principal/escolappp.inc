<?php
//
// $Id$
//



header("Cache-Control: no-cache, must-revalidate"); // HTTP/1.1
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT"); // Date in the past


cte_verificaSessao();


require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/ItemPPP.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/ConteudoPPP.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/AreaCurso.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/CursoTecnico.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/ConteudoPPPCursoTecnico.php";

if ($_REQUEST['carregarCursosTecnicos'] && $_REQUEST['areid']) {
    $area   = new AreaCurso($_REQUEST['areid']);
    $cursos = $area->carregarCursosTecnicos();

    //echo "var Cursos = new Array();\n";
    foreach ($cursos as $curso) {
        //echo 'Cursos.push(new Array(' , $curso->getPrimaryKey() , ', "', $curso->crstitulo , '"));';
        echo '<option id="curso_' ,$curso->getPrimaryKey(), '" value="' , $curso->getPrimaryKey() , '" title="' , $curso->crstitulo , '">' , $curso->crstitulo , "</option>\n";
    }

    die();
}

if ($_REQUEST['exibirDetalhesCurso'] && $_REQUEST['crsid']) {
    $curso = new CursoTecnico($_REQUEST['crsid']);
    $curso->carregarAreaCurso();
?><html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
  </head>
  <body style="margin:10px; padding:0; background-color: #f5f5f5; background-image: url(../imagens/fundo.gif);">
    <table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#f5f5f5" class="tabela" style="width: 95%;">
      <tr>
        <td style="padding: 5px; background-color:#fafafa;" width="35%">
          <table>
            <tr>
              <td style="font-weight: bold"><?php echo $curso->areaCurso->aretitulo; ?></td>
            </tr>
            <tr>
              <td><?php echo $curso->areaCurso->aredsc; ?></td>
            </tr>
          </table>
        </td>
        <td style="padding: 5px; background-color:#fafafa; border-left: 1px dotted #ccc; vertical-align: top" width="65%">
          <h2 style="text-align: right; margin-bottom: 0"><span><?php echo $curso->crstitulo; ?></span></h2>
          <span style="border-bottom: 1px solid #ddd; display: block; text-align: right"><?php echo $curso->crscargahoraria; ?> horas</span>

          <p style="padding: 7px;">
            <span style="border-bottom: 1px solid #ddd; display: block; font-weight: bold;">Possibilidades de temas a serem abordados na formação</span>
            <?php echo $curso->crstema; ?>
          </p>

          <p style="padding: 7px;">
            <span style="border-bottom: 1px solid #ddd; display: block; font-weight: bold;">Possibilidades de atuação</span>
            <?php echo $curso->crsatuacao; ?>
          </p>

          <p style="padding: 7px;">
            <span style="border-bottom: 1px solid #ddd; display: block; font-weight: bold;">Infra-estrutura recomendada</span>
            <ul style="margin: 0; padding: 0 20px">
              <li><?php echo str_replace('. ', '.</li><li>', $curso->crsinfraestrutura); ?></li>
            </ul>
          </p>
        </td>
      </tr>
      <tr>
        <td style="text-align: center" colspan="2">
          <input type="button" value="Fechar janela" onclick="self.close();" />
        </td>
      </tr>
    </table>
  <body>
</html>

<?php
    die();
}

$item     = new ItemPPP($_REQUEST['ippid']);
$conteudo = ConteudoPPP::carregarPorItemPPPEntidade($item, $_REQUEST['entid']);
$itemPai  = new ItemPPP($item->ipppai);
$cpptexto = $conteudo->cpptexto;
$txt      = campo_textarea('cpptexto', 'S', 'S', '', 95, 10, 4000);

if ($_REQUEST['salvar'] == 1) {
    $conteudo->ippid    = $_REQUEST['ippid'];
    $conteudo->entid    = $_REQUEST['entid'];
    $conteudo->cpptexto = is_array($_REQUEST['cpptexto']) ? implode("|", $_REQUEST['cpptexto']) : $_REQUEST['cpptexto'];

    try {
        if ($_REQUEST['ipptiporesposta'] == 5) {
            ConteudoPPPCursoTecnico::excluirTodosPorConteudoPPP($conteudo);

            if (array_key_exists('cpptexto', $_REQUEST)) {
                $ConteudoPPPCursoTecnico        = new ConteudoPPPCursoTecnico();

                foreach ($_REQUEST['cpptexto'] as $crsid) {
                    if ($crsid == '')
                        continue;

                    $ConteudoPPPCursoTecnico->crsid = $crsid;
                    $ConteudoPPPCursoTecnico->cppid = $conteudo->getPrimaryKey();

                    $ConteudoPPPCursoTecnico->save();
                }
            }
        } elseif ($_REQUEST['ipptiporesposta'] == 1) {
        }

        if (trim($conteudo->cpptexto) != '' || $conteudo->docid != '') {
            $conteudo->BeginTransaction();
            $conteudo->salvar();
            $conteudo->Commit();

            $salvo    = true;
            $cpptexto = $conteudo->cpptexto;
            $txt      = campo_textarea('cpptexto', 'S', 'S', '', 95, 10, 4000);
        }
    } catch (Exception $e) {
        $conteudo->Rollback();
        alert('Não foi possível salvar os dados!\\n\\nErro:\\n' . $e->getMessage());
    }
}





?><html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
        function salvarConteudoPPP(frm, close)
        {
            var frm = $(frm);

            if ($('cpptexto').tagName.toUpperCase() == 'SELECT') {
                for (var i = 0; i < $('cpptexto').options.length; i++)
                    $('cpptexto').options[i].selected = true;
            }

            var docid = document.getElementById('docid');
            if (docid != null) {
                if (docid.value == '') {
                    alert('O campo arquivo é obrigatório!');
                    return false;
                }
            }

            frm.submit();
        }

        function fecharJanela()
        {
            window.opener.location.reload();
            self.close();
            return false;
        }

        var Area            = null;
        var AreaSelecionada = null;

        function carregarCursoPorArea()
        {
            var req = new Ajax.Request('cte.php?modulo=principal/escolappp&acao=A', {
                method: 'post',
                parameters: '&carregarCursosTecnicos=true&areid=' + $F('areacurso'),
                onComplete: function (res)
                {
                    $('cursotecnico').innerHTML = '';
                    Element.insert($('cursotecnico'), {bottom: res.responseText});

                    for (var i = 0; i< $('cursotecnico').options.length; i++) {
                        $('cursotecnico').options[i].selected = false;
                    }

                    AreaSelecionada = $F('areacurso');
                }
            });
        }

        function adicionarCursoTecnico(crsid)
        {
            if ($('cursotecnico').selectedIndex == -1 && !crsid) {
                return false;
            }

            var i = $('cpptexto').options.length;

            if (crsid) {
                var cppid = 'curso_' + crsid;
            } else {
                var cppid = 'curso_' + $F('cursotecnico');
            }

            $('cpptexto').options[i] = new Option($(cppid).innerHTML,
                                                  $('cursotecnico').value,
                                                  false,
                                                  false);

            $('cpptexto').options[i].setAttribute('id', 'cpptexto_' + $('cursotecnico').value);
            $('cursotecnico').removeChild($(cppid));
        }

        function removerCursoTecnico(crsid)
        {
            if ($('cpptexto').selectedIndex == -1 && !crsid) {
                return false;
            }

            var i = $('cursotecnico').options.length;

            if (crsid) {
                var cppid = 'cpptexto_' + crsid;
            } else {
                var cppid = 'cpptexto_' + $F('cpptexto');
            }

            if (Area == AreaSelecionada && $('cursotecnico').options.length > 0) {
                $('cursotecnico').options[i] = new Option($(cppid).innerHTML,
                                                          $('cpptexto').value,
                                                          false,
                                                          false);

                $('cursotecnico').options[i].setAttribute('id', 'curso_' + $('cpptexto').value);
            }

            $('cpptexto').removeChild($(cppid));
        }

        function exibirDetalhesCurso()
        {
            return windowOpen('cte.php?modulo=principal/escolappp&acao=A&exibirDetalhesCurso=true&crsid=' + $F('cursotecnico'), 'detalhesCursoTecnico', 'height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no' );
        }


        function exibirDetalhesArea()
        {
            return windowOpen('cte.php?modulo=principal/escolappp&acao=A&exibirDetalhesArea=true&crsid=' + $F('areacurso'), 'detalhesCursoTecnico', 'height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no' );
        }
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #f5f5f5; background-image: url(../imagens/fundo.gif);">
    <form id="frmPPP" action="cte.php?modulo=principal/escolappp&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&ippid=<?php echo $item->ippid ?>" method="post" onsubmit="return salvarConteudoPPP('frmPPP', true);" enctype="multipart/form-data">
      <table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#f5f5f5" class="tabela" style="width: 95%;">
        <tr>
          <td style="padding: 5px; background-color:#fafafa;" colspan="3">
            <?php echo "<h3 style=\"color: #404040;\">" , $itemPai->ipptitulo , "</h3>\n" , $itemPai->ippdsc; ?>
          </td>
        </tr>

        <tr>
          <td style="padding: 5px; background-color:#fafafa;" colspan="3">
            <?php echo "<h4>" , $item->ipptitulo , "</h4>" , $item->ippdsc; ?>
            <?php if (isset($salvo) && $salvo === true) echo "<span style=\"display: block; text-align: center; color: #900; font-weight: bold\">Dados salvos com sucesso!</span>\n"; ?>
          </td>
        </tr>

<?php
    if ($_REQUEST['ipptiporesposta'] == 0) {
?>
        <tr>
          <td style="padding: 5px; background-color:#fafafa;" colspan="3">
          <?php echo $txt; ?>
          </td>
        </tr>
<?php
    } // ! if ($_REQUEST['ipptiporesposta'] == 0)

    elseif ($_REQUEST['ipptiporesposta'] == 1) {
?>
        <tr>
          <td colspan="3">
            <table>
              <tr>
                <td align='right' class="SubTituloDireita" style="width:25%">Arquivo:</td>
                <td>
                  <input type="file" name="cpptexto"/>
                  <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                </td>
              </tr>

              <tr>
                <td align='right' class="SubTituloDireita" style="width:25%">Tipo:</td>
                <td>
                  <select style="width: 200px;" class="CampoEstilo" name="taaid">
                    <optgroup label="Instrumento Legal">
<?php
    $sql = "select taaid, taadescricao from pde.tipoanexoatividade where taalegal = true order by taadescricao asc";
    foreach ($db->carregar($sql) as $tipo) {
        echo '                      <option value="' , $tipo['taaid'] , '">' , $tipo['taadescricao'] , "</option>\n";
    }
?>
                    </optgroup>
                    <optgroup label="Instrumento de Trabalho">
<?php
    $sql = "select taaid, taadescricao from pde.tipoanexoatividade where taalegal = false order by taadescricao asc";
    foreach ($db->carregar($sql) as $tipo) {
        echo '                      <option value="' , $tipo['taaid'] , '">' , $tipo['taadescricao'] , "</option>\n";
    }
?>
                    </optgroup>
                  </select>
                </td>
              </tr>

              <tr>
                <td align='right' class="SubTituloDireita" style="width:25%">Descrição:</td>
                <td><?php echo campo_textarea('anedescricao', 'S', 'S', '', 70, 2, 250 ); ?></td>
              </tr>
            </table>
          </td>
        </tr>
<?php
    } // !elseif ($_REQUEST['ipptiporesposta'] == 1)
    elseif ($_REQUEST['ipptiporesposta'] == 2) {
        $cpptexto = explode("|", $conteudo->cpptexto);

?>
          <td colspan="3">
            <table width="100%">
              <tr>
                <td align='right' class="SubTituloDireita" style="width:125px">Cidade:</td>
                <td>
                  <input type="text" name="cpptexto[]" size="5" value="<?php echo $cpptexto[0]; ?>"/> 
                  <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                </td>
              </tr>
              <tr>
                <td align='right' class="SubTituloDireita" style="width:125px">Estado:</td>
                <td>
                  <input type="text" name="cpptexto[]" size="5" value="<?php echo $cpptexto[1]; ?>"/>
                  <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                </td>
              </tr>
            </table>
            <input type="hidden" name="ipptiporesposta" value="2" />
          </td>
<?php
    } // !elseif ($_REQUEST['ipptiporesposta'] == 2)
    elseif ($_REQUEST['ipptiporesposta'] == 3) {
        $cpptexto = explode("|", $conteudo->cpptexto);
?>
          <td colspan="3">
            <table width="100%">
              <tr>
                <td align='right' class="SubTituloDireita" style="width:125px">Escola:</td>
                <td>
                  <input type="text" name="cpptexto[]" size="5" value="<?php echo $cpptexto[0]; ?>"/> 
                  <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                </td>
              </tr>
              <tr>
                <td align='right' class="SubTituloDireita" style="width:125px">Estado:</td>
                <td>
                  <input type="text" name="cpptexto[]" size="5" value="<?php echo $cpptexto[1]; ?>"/>
                  <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                </td>
              </tr>
            </table>
            <input type="hidden" name="ipptiporesposta" value="3" />
          </td>

<?php
    } // !elseif ($_REQUEST['ipptiporesposta'] == 2)
    elseif ($_REQUEST['ipptiporesposta'] == 4) {

?>
          <td colspan="3">
            <input type="radio" name="cpptexto" value="sim" id="sim" <?php echo ($cpptexto == 'sim' ? 'checked="checked"' : ''); ?> /><label for="sim">Sim</label>
            <input type="radio" name="cpptexto" value="nao" id="nao" <?php echo ($cpptexto == 'nao' ? 'checked="checked"' : ''); ?> /><label for="nao">Não</label>
            <input type="hidden" name="ipptiporesposta" value="4" />
            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
          </td>
<?php
    } // !elseif ($_REQUEST['ipptiporesposta'] == 4)
    elseif ($_REQUEST['ipptiporesposta'] == 5) {

?>
          <td colspan="3">
            <table width="100%">
              <tr>
                <td width="40%" style="text-align: center;font-weight: bold">Área</td>
                <td width="60%" style="text-align: center;font-weight: bold">Curso Técnico<br /></td>
              </tr>

              <tr>
                <td colspan="2"style="text-align: center;"><small style="color: blue">Dê dois cliques para exibir os detalhes da área ou curso técnico</small></td>
              </tr>

              <tr>
                <td width="40%">
                  <select name="areacurso" id="areacurso" multiple="true" onclick="return carregarCursoPorArea();" ondblclick="exibirDetalhesArea();" style="height: 30ex; width: 200px;">
<?php
    $area = new AreaCurso();
    $arr  = $area->carregarColecao();

    foreach ($arr as $areaCurso) {
        echo '                  <option value="' , $areaCurso->getPrimaryKey()
            ,'" title="' , $areaCurso->aretitulo , '">' , $areaCurso->aretitulo , "</option>\n";
    }
?>
                  </select>
                </td>
                <td width="60%">
                  <select name="cursotecnico" id="cursotecnico" multiple="true" style="height: 30ex; width: 290px;" ondblclick="exibirDetalhesCurso();">
                  </select>
                </td>
              </tr>

              <tr>
                <td colspan="2" style="text-align: right">
                  <a href="javascript:void(0);" onclick="return adicionarCursoTecnico();">Adicionar</a>
                </td>
              </tr>

              <tr>
                <td colspan="2" style="text-align: center;font-weight: bold">Cursos selecionados</td>
              </tr>
              <tr>
                <td colspan="2">
                  <select name="cpptexto[]" id="cpptexto" multiple="true" style="height: 20ex; width: 496px;">
<?php

$curso = ConteudoPPPCursoTecnico::carregarPorConteudoPPP($conteudo);
foreach ($curso as $arr) {
    $cursoTecnico = new CursoTecnico($arr[0]);
    echo '<option id="cpptexto_' ,$cursoTecnico->getPrimaryKey(), '" value="' , $cursoTecnico->getPrimaryKey() , '" title="' , $cursoTecnico->crstitulo , '">' , $cursoTecnico->crstitulo , "</option>\n";
}

?>
                  </select>
                </td>
              </tr>
              <tr>
                <td colspan="2" style="text-align: right">
                  <a href="javascript:void(0);" onclick="return removerCursoTecnico();">Remover</a>
                </td>
              </tr>

            </table>
            <input type="hidden" name="ipptiporesposta" value="5" />
          </td>

<?php
    } // !elseif ($_REQUEST['ipptiporesposta'] == 5)

?>
        <tr>
          <td style="padding: 5px; background-color:#fafafa; text-align: center" width="20%">
          <?php
          $nItemPPP = $item->anterior();
          if ($nItemPPP !== -1) {
              echo '<a href="cte.php?modulo=principal/escolappp&acao=A&ipptiporesposta=' ,$nItemPPP->ipptiporesposta, '&entid=' , $_REQUEST['entid'] , '&ippid=' , $nItemPPP->ippid , '"><< Anterior</a>';
          } else {
              echo '<< Anterior';
          }
          ?>
          </td>

          <td style="padding: 5px; background-color:#fafafa; text-align: center" width="60%">
            <input type="hidden" name="ippid" id="ippid" value="<?php echo $item->ippid; ?>" />
            <input type="hidden" name="entid" value="<?php echo $_REQUEST['entid']; ?>" />
            <input type="hidden" name="cppid" value="<?php echo $conteudo->getPrimaryKey(); ?>" />
            <input type="hidden" name="salvar" value="1" />
            <input type="hidden" name="close" id="close" value="0" />

              <input type="submit" value="Salvar" />
              <input type="button" value="Fechar" onclick="return fecharJanela();" />
          </td>

          <td style="padding: 5px; background-color:#fafafa; text-align: center" width="20%">
          <?php
          $nItemPPP = $item->proximo();
          if ($nItemPPP !== -1) {
              echo '<a href="cte.php?modulo=principal/escolappp&acao=A&ipptiporesposta=' ,$nItemPPP->ipptiporesposta, '&entid=' , $_REQUEST['entid'] , '&ippid=' , $nItemPPP->ippid , '">Próxima >></a>';
          } else {
              echo 'Próxima >>';
          }
          ?>
          </td>
        </tr>
      </table>
    </form>
  </body>
</html>

