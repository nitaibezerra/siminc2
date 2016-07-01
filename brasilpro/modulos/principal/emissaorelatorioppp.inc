<?php
//
// $Id$
//



header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");


cte_verificaSessao();


require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/ItemPPP.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/ConteudoPPP.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/AreaCurso.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/CursoTecnico.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/ConteudoPPPCursoTecnico.php";

if (array_key_exists('entid', $_REQUEST)) {
    $entid    = $_REQUEST['entid'];
    $entidade = new Entidade($entid);

    if ($entidade->getPrimaryKey() === null) {
        echo '<script type="text/javascript">'
            ,'alert("Você deve selecionar uma entidade válida!");'
            ,'self.close();'
            ,'</script>';

        exit;
    }
}



?><html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <!-- <link rel="stylesheet" type="text/css" href="../includes/print.css"/> -->

    <style type="text/css">
    body {
        font-size: 85%;
        color: #000;
        background-color: #fff;
    }

    h2, h3, h4 {
        text-align: center;
        margin: 0px auto;
    }

    h3 {
        border-bottom: 2px solid #aaa;
    }

    p, p span {
        padding: 0px;
    }

    .ipptitulo {
        text-align: justify;
        font-weight: bold;
        display: block;
    }

    .ippdsc {
        padding-left: 10px;
        display: block;
        font-size: 0.9em;
        position: relative;
    }

    .cpptexto {
        padding: 0px 5px 0px 5px;
        margin: 5px 25px 10px 25px;
        font-style: italic;
        border-left: 1px solid #ccc;
    }

    .nivel1 {
        margin: 0px;
        padding: 0px;
    }

    .nivel1 > .ipptitulo {
        background-color: #ccc;
        padding: 2px;
    }

    .nivel3 {
        margin: 0px;
        padding: 0px;
    }

    @media print {
        .buttons {
            display: none;
        }
    }
    </style>
  </head>
  <!--
    <body style="margin:10px; padding:0; background-color: #f5f5f5; background-image: url(../imagens/fundo.gif);">
    -->
  <body>
    <div>
      <div class="buttons" style="text-align: center;">
        <input type="button" value="Imprimir" onclick="window.print(); self.close();" />
        <input type="button" value="Fechar" onclick="return self.close();" /><hr />
      </div>
      <h2><span>Relatório do Projeto Político Pedagógico</span></h2>
      <?php
        echo "      <h3><span>" , $entidade->entnome , "</span></h3>\n";

        foreach (ItemPPP::carregarFilhos() as $itemPPP) {

            $filhos1Nivel = ItemPPP::carregarFilhos($itemPPP->ippid);

            echo "      <div class=\"nivel1\">\n"
                ,"        <span class=\"ipptitulo\">" , $itemPPP->ipptitulo , "</span>\n"
                ,"        <span class=\"ippdsc\">" , $itemPPP->ippdsc , "</span>\n";

            if (sizeof($filhos1Nivel) == 0) {
                if ($itemPPP->ippconteudo != 0) {
                    $cpptexto = ConteudoPPP::carregarPorItemPPPEntidade($itemPPP, $entid)->cpptexto;
                } else {
                    $cpptexto = null;
                }

                if ($cpptexto != null)
                    echo "        <p class=\"cpptexto\">" , $cpptexto , "</p>\n";
                else
                    echo "        <p class=\"cpptexto\" style=\"margin-left: 15px;\">Sem resposta</p>\n";
            } else {
                foreach ($filhos1Nivel as $itemPPP1Nivel) {
                    $filhos2Nivel = ItemPPP::carregarFilhos($itemPPP1Nivel->ippid);

                    echo "      <div class=\"nivel2\">\n"
                        ,"        <span class=\"ipptitulo\" style=\"padding-left: 10px;\">" , $itemPPP1Nivel->ipptitulo , "</span>\n"
                        ,"        <span class=\"ippdsc\" style=\"padding-left: 15px;\">" , $itemPPP1Nivel->ippdsc , "</span>\n";

                    if (sizeof($filhos2Nivel) == 0) {
                        if ($itemPPP1Nivel->ippconteudo != null) {
                            $cpptexto = ConteudoPPP::carregarPorItemPPPEntidade($itemPPP1Nivel, $entid)->cpptexto;
                        } else {
                            $cpptexto = null;
                        }

	                    if ($itemPPP1Nivel->ipptiporesposta == 0) {
			                if ($cpptexto != null)
			                    echo "        <p class=\"cpptexto\">" , $cpptexto , "</p>\n";
			                else
			                    echo "        <p class=\"cpptexto\" style=\"margin-left: 15px;\">Sem resposta</p>\n";
	                    } elseif ($itemPPP1Nivel->ipptiporesposta == 5) {
                            $conteudo = ConteudoPPP::carregarPorItemPPPEntidade($itemPPP1Nivel, $entid);
                            $cursos   = ConteudoPPPCursoTecnico::carregarPorConteudoPPP($conteudo);

                            if (sizeof($cursos) > 0) {
                                echo '        <p class="cpptexto"><ul class="cpptexto"><ul>';
                                $ultimaArea = null;
                                $header     = false;

                                foreach ($cursos as $curso) {
                                    $cursoTecnico = new CursoTecnico($curso[0]);
                                    $cursoTecnico->carregarAreaCurso();

                                    /*!@
                                    if ($ultimaArea != $cursoTecnico->areid) {
                                        echo '<strong>' , $cursoTecnico->areaCurso->aretitulo , '</strong>'
                                            ,'<span style="display: block; font-size: 0.9em; padding-bottom: 5px;">' , $cursoTecnico->areaCurso->aredsc , '</span><ul>'
                                            ,'<li><span>' , $cursoTecnico->crstitulo , '</span><small style="display: block; text-align: justify">' , $cursoTecnico->crsdsc , '</small></li>';
                                    } else {
                                        echo '<li><span>' , $cursoTecnico->crstitulo , '</span><small style="display: block; text-align: justify">' , $cursoTecnico->crsdsc , '</small></li>';
                                    }

                                    //                                      */

                                    echo '<li style="padding-bottom: 5px;">'
                                        ,'<span>' , $cursoTecnico->areaCurso->aretitulo , ' - ' , $cursoTecnico->crstitulo , '</span>'
                                        ,'<small style="display: block; text-align: justify">' , $cursoTecnico->crsdsc    , '</small>'
                                        ,'</li>';
                                }

                                echo "</ul>\n";

                            } else
                                echo '        <p class="cpptexto" style="margin-left: 15px;">Sem resposta</p>' , "\n";
                        } else {
                            if ($cpptexto == null) {
                                echo '        <p class="cpptexto" style="margin-left: 15px;">Sem resposta</p>' , "\n";
                                continue;
                            }

                            echo '        <p class="cpptexto" style="margin-left: 15px;">';
                            if ($itemPPP1Nivel->ipptiporesposta == 2) {
                                $cpptexto = explode("|", $cpptexto);
                                echo 'Estado: ' , $cpptexto[0] , '<br />' , 'Município: ' , $cpptexto[1];
                            } elseif ($itemPPP1Nivel->ipptiporesposta == 3) {
                                $cpptexto = explode("|", $cpptexto);
                                echo 'Escola: ' , substr($cpptexto[0], 0, 5) , '<br />' , 'Estado: ' , substr($cpptexto[1], 0, 5);
                            }

                            echo "</p>\n";
                        }
                    } else {
                        foreach ($filhos2Nivel as $itemPPP2Nivel) {
                            echo "      <div class=\"nivel3\">\n"
                                ,"        <span class=\"ipptitulo\" style=\"padding-left: 15px; font-weight: normal;\">" , $itemPPP2Nivel->ipptitulo , "</span>\n"
                                ,"        <span class=\"ippdsc\" style=\"padding-left: 20px; font-weight: normal;\">" , $itemPPP2Nivel->ippdsc , "</span>\n";

                            if ($itemPPP2Nivel->ippconteudo != 0) {
                                $cpptexto = ConteudoPPP::carregarPorItemPPPEntidade($itemPPP2Nivel, $entid)->cpptexto;
                            } else {
                                $cpptexto = null;
                            }

                            if ($cpptexto != null)
                                echo "        <p class=\"cpptexto\" style=\"margin-left: 25px;\">" , $cpptexto , "</p>\n";
                            else
                                echo "        <p class=\"cpptexto\" style=\"margin-left: 25px;\">Sem resposta</p>\n";

                            echo "        </div>\n";
                        }
                    }

                    echo "        </div>\n";
                }
            }

            echo "        </div>\n";
        }
        ?>

        <div class="buttons" style="text-align: center;">
          <input type="button" value="Imprimir" onclick="window.print(); self.close();" />
          <input type="button" value="Fechar" onclick="return self.close();" />
        </div>
    </div>
  </body>
</html>

