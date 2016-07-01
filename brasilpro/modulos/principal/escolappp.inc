<?php
//
// $Id$
//

header("Cache-Control: no-cache, must-revalidate");
header("Expires: Mon, 26 Jul 1997 05:00:00 GMT");

cte_verificaSessao();


require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/arquivo.inc";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Arquivo.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/EntidadeDetalhe.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/ItemPPP.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/ConteudoPPP.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/AreaCurso.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/CursoTecnico.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/ConteudoPPPCursoTecnico.php";

if ($_REQUEST['carregarCursosTecnicos'] && $_REQUEST['areid']) {
	if(!strpos($_REQUEST['areid'],",")){
	    $area   = new AreaCurso($_REQUEST['areid']);
	    $cursos = $area->carregarCursosTecnicos();
	    foreach ($cursos as $curso) {
	        echo '<option id="curso_' ,$curso->getPrimaryKey(), '" value="' , $curso->getPrimaryKey() , '" title="' , $curso->crstitulo , '">' , $curso->crstitulo , "</option>\n";
	    }
	}else{
		echo '<option id="" value="" title="Selecione apenas um área ">Selecione apenas uma área </option>\n';
	   
	}
    exit;
}


if ($_REQUEST['downloadArquivo'] && $_REQUEST['cppid']) {
    $conteudo = new ConteudoPPP($_REQUEST['cppid']);
    $arquivo  = new Arquivo($conteudo->arqid);

    if (is_file(APPRAIZ . 'arquivos/anexos/anexoppp-' . $arquivo->getPrimaryKey())) {
        header('Cache-Control: no-cache, must-revalidate');
        header('Expires: Mon, 26 Jul 1997 05:00:00 GMT');
        header('Content-Length: '                            . $arquivo->arqtamanho);
        header('Content-type: '                              . $arquivo->arqtipo);
        header('Content-Disposition: attachment; filename="' . $arquivo->arqnome. '"');
        readfile(APPRAIZ . 'arquivos/anexos/anexoppp-'       . $arquivo->getPrimaryKey());

        exit;
    }
}


if ($_REQUEST['excluirArquivo'] && $_REQUEST['cppid']) {
    $conteudo = new ConteudoPPP($_REQUEST['cppid']);
    $arquivo  = new Arquivo($conteudo->arqid);

    try {
        $conteudo->BeginTransaction();

        $conteudo->arqid    = 'null';
        $conteudo->cpptexto = '';
        $conteudo->save();

        $arquivo->excluir();

        if (!is_file(APPRAIZ . 'arquivos/anexos/anexoppp-' . $arquivo->getPrimaryKey()) ||
            !unlink(APPRAIZ  . 'arquivos/anexos/anexoppp-' . $arquivo->getPrimaryKey()))
        {
            throw new Exception("Não foi possível excluir o arquivo.\nTente novamente em alguns instantes.");
        }

        $conteudo->Commit();

        die(0);
    } catch (Exception $e) {
        $conteudo->Rollback();
        die($e->getMessage());
    }
}

if ($_REQUEST['exibirDetalhesCurso'] && $_REQUEST['crsid']) {
    if (is_array($_REQUEST['crsid'])) {
        $crsid = $_REQUEST['crsid'][0];
    } else {
        $crsid = $_REQUEST['crsid'];
    }

    $curso = new CursoTecnico($crsid);
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
    <script type="text/javascript" src="../includes/estouvivo.js"></script>

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
} elseif ($_REQUEST['exibirDetalhesArea'] && $_REQUEST['areid']) {
    if (is_array($_REQUEST['areid'])) {
        $areid = $_REQUEST['areid'][0];
    } else {
        $areid = $_REQUEST['areid'];
    }

    $areaCurso = new AreaCurso($areid);
?><html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="Mon, 26 Jul 1997 05:00:00 GMT">
    <title><?= $titulo ?></title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript" src="../includes/estouvivo.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
  </head>
  <body style="margin:10px; padding:0; background-color: #f5f5f5; background-image: url(../imagens/fundo.gif);">
    <table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#f5f5f5" class="tabela" style="width: 95%;">
      <tr>
        <td>
          <span style="padding: 5px; text-align: right; font-weight: bold; font-size: 1.1em; display: block"><?php echo $areaCurso->aretitulo; ?></span>
        </td>
      <tr>
        <td>
          <?php echo $areaCurso->aredsc; ?>
        </td>
      </tr>
      <tr>
        <td style="text-align: center">
          <input type="button" value="Fechar janela" onclick="self.close();" />
        </td>
      </tr>
    </table>
  <body>
</html>

<?php
    die();
}

global $cpptexto;

$item     = new ItemPPP($_REQUEST['ippid']);
$conteudo = ConteudoPPP::carregarPorItemPPPEntidade($item, $_REQUEST['entid']);

if($_REQUEST['entid']) {

	$sql = "SELECT entdoferta_ept FROM entidade.entidadedetalhe WHERE entid = '". $_REQUEST['entid'] ."'";
	$ent = current((Array) $db->carregar($sql));
	if($ent['entdoferta_ept'] == 'f' && $item->ippbloqdifept == 't') {
		$bloqueiagravacao = true;
	} else {
		$bloqueiagravacao = false;
	}
} else {
	$bloqueiagravacao = true;
}

if ($item->ipppai)
    $itemPai = new ItemPPP($item->ipppai);

$cpptexto = $conteudo->cpptexto;
$txt      = campo_textarea('cpptexto', 'S',(($bloqueiagravacao)?'N':'S'), '', 95, 10, 4000);

if ($_REQUEST['salvar'] == 1) {
    $conteudo->ippid    = $_REQUEST['ippid'];
    $conteudo->entid    = $_REQUEST['entid'];
    if (get_magic_quotes_gpc() == 1)
        $conteudo->cpptexto = '' . (is_array($_REQUEST['cpptexto']) ? implode("|", $_REQUEST['cpptexto']) : stripslashes($_REQUEST['cpptexto']));
    else
        $conteudo->cpptexto = '' . (is_array($_REQUEST['cpptexto']) ? implode("|", $_REQUEST['cpptexto']) : $_REQUEST['cpptexto']);

    if ($conteudo->getPrimaryKey() === null) {
        $conteudo->save();
    }

    $conteudo->BeginTransaction();

    try {
        if ($_REQUEST['ipptiporesposta'] == 5) {
            ConteudoPPPCursoTecnico::excluirTodosPorConteudoPPP($conteudo);

            if (array_key_exists('cpptexto', $_REQUEST)) {
                $ConteudoPPPCursoTecnico = new ConteudoPPPCursoTecnico();

                foreach ($_REQUEST['cpptexto'] as $crsid) {
                    if ($crsid == '')
                        continue;

                    $ConteudoPPPCursoTecnico->crsid = $crsid;
                    $ConteudoPPPCursoTecnico->cppid = $conteudo->getPrimaryKey();

                    $ConteudoPPPCursoTecnico->save();
                }
            }
        } elseif ($_REQUEST['ipptiporesposta'] == 1) {
            $conteudo->carregarArquivo();
            $arquivo = $conteudo->arquivo;

            if (is_array($_FILES['arqid']) && $_FILES['arqid']['name'] != '') {
                $anexo = APPRAIZ . 'arquivos/anexos/anexoppp-' . $arquivo->getPrimaryKey();

                if ($arquivo->getPrimaryKey() !== null && is_file($anexo)) {
                    unlink($anexo);
                }

                $arquivo->arqnome      = $_FILES['arqid']['name'];
                $arquivo->arqdescricao = '';
                $arquivo->arqextensao  = substr(0, 3, $_FILES['arqid']['name']);
                $arquivo->arqtipo      = $_FILES['arqid']['type'];
                $arquivo->arqtamanho   = $_FILES['arqid']['size'];
                $arquivo->arqdata      = date('Y-m-d');
                $arquivo->arqhora      = date('H:i:s');
                $arquivo->arqstatus    = 1;
                $arquivo->usucpf       = $_SESSION['usucpf'];

                $arquivo->save();
                $caminho = $anexo;

                if (!move_uploaded_file($_FILES['arqid']['tmp_name'], $caminho)) {
                    throw new Exception('Não foi possível fazer upload do arquivo.');
                }

                $conteudo->arqid = $arquivo->getPrimaryKey();
                $conteudo->save();
            } else {
                $arquivo = $conteudo->arquivo;

                if ($arquivo->getPrimaryKey() !== null) {
                    $conteudo->arqid    = 'null';
                    $conteudo->save();
                    $conteudo->arqid    = null;

                    $arquivo->excluir();

                    if (is_file(APPRAIZ . 'arquivos/anexos/anexoppp-' . $arquivo->getPrimaryKey())) {
                        if (!unlink(APPRAIZ  . 'arquivos/anexos/anexoppp-' . $arquivo->getPrimaryKey()))
                            throw new Exception('Erro ao excluir o arquivo');
                    }
                }
            }
        }

        if (trim($conteudo->cpptexto) != '') {
            $conteudo->save();
            $conteudo->Commit();

            $salvo    = true;
            $cpptexto = $conteudo->cpptexto;
            $txt      = campo_textarea('cpptexto', 'S', 'S', '', 95, 10, 4000);
        }

        $conteudo->Commit();
    } catch (Exception $e) {
        $conteudo->Rollback();
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
    <script type="text/javascript" src="../includes/estouvivo.js"></script>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <script type="text/javascript">
        function salvarConteudoPPP(frm, close)
        {
            var frm = $(frm);

            var elm = Form.getElements(frm);
            for (var i = 0; i < elm.length; i++) {
                if (elm[i].tagName.toUpperCase() != 'SELECT' && /cpptexto/.test(elm[i].getAttribute('name')) && elm[i].value == '') {
                    alert('Por favor, preencha os campos obrigatórios.');
                    return false;
                }
            }

            if ($('cpptexto')) {
                if ($('cpptexto').tagName.toUpperCase() == 'SELECT') {
                    for (var i = 0; i < $('cpptexto').options.length; i++)
                        $('cpptexto').options[i].selected = true;
                }
            }
			if($('ipptiporesposta')) {
            if ($('ipptiporesposta').value == '1') {
                if ($('sim').checked && $('arqid').value == '') {
                    alert('O campo Arquivo é obrigatório!');
                    return false;
                }
            }
            }

            return true;
        }

        var Area            = null;
        var AreaSelecionada = null;

        function carregarCursoPorArea()
        {
            var areacurso = $F('areacurso');
            Element.show('aguarde');
            Form.disable('areacurso');

            var req = new Ajax.Request('brasilpro.php?modulo=principal/escolappp&acao=A', {
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

                    Element.hide('aguarde');
                    Form.enable('areacurso');
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
            return windowOpen('brasilpro.php?modulo=principal/escolappp&acao=A&exibirDetalhesCurso=true&crsid=' + $F('cursotecnico'), 'detalhesCursoTecnico', 'height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no' );
        }

        function exibirDetalhesArea()
        {
            return windowOpen('brasilpro.php?modulo=principal/escolappp&acao=A&exibirDetalhesArea=true&areid=' + $F('areacurso'), 'detalhesCursoTecnico', 'height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no' );
        }

        function excluirArquivo(cppid)
        {
            var req = new Ajax.Request('brasilpro.php?modulo=principal/escolappp&acao=A', {
                method:     'post',
                parameters: '&excluirArquivo=true&cppid=' + cppid,
                onComplete: function (res)
                {
                    if (res.responseText == 0) {
                        Element.hide('arqtitle');
                        Element.hide('arqlist');
                        alert('Arquivo removido com sucesso!');
                    } else {
                        alert(res.responseText);
                    }
                }
            });
        }
    </script>
  </head>
  <body style="margin:10px; padding:0; background-color: #f5f5f5; background-image: url(../imagens/fundo.gif);" onclose="this.opener.location.reload();">
    <div id="aguarde" style="background-color:#ffffff;position:absolute;color:#000033;top:30%;left:20%;border:2px solid #cccccc; width:300;font-size:12px;z-index:0;padding:15px">
      <br /><img src="../imagens/wait.gif" border="0" align="middle"> Aguarde! Carregando Dados...<br /><br />
    </div>

    <form id="frmPPP" action="brasilpro.php?modulo=principal/escolappp&acao=A&entid=<?php echo $_REQUEST['entid']; ?>&ippid=<?php echo $item->ippid ?>" method="post" onsubmit="return salvarConteudoPPP('frmPPP', true);" enctype="multipart/form-data">
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
        $cpptexto = $conteudo->cpptexto;
        $conteudo->carregarArquivo();

?>
        <tr>
          <td colspan="3">
            <input type="radio" name="cpptexto" value="sim" id="sim" <?php echo ($cpptexto == 'sim' ? 'checked="checked"' : ''); echo (($bloqueiagravacao)?'disabled':''); ?> onclick="Element.show('arqinput')" /><label for="sim">Sim</label>
            <input type="radio" name="cpptexto" value="nao" id="nao" <?php echo ($cpptexto == 'nao' ? 'checked="checked"' : ''); echo (($bloqueiagravacao)?'disabled':''); ?> onclick="Element.hide('arqinput')" /><label for="nao">Não</label>
            <input type="hidden" name="ipptiporesposta" id="ipptiporesposta" value="1" />
          </td>
        </tr>

        <tr id="arqinput">
          <td align='right' class="SubTituloDireita" style="width:25%">Arquivo:</td>
          <td colspan="2">
            <input type="file" name="arqid" id="arqid" <?  echo (($bloqueiagravacao)?'disabled':''); ?>/>
          </td>
        </tr>

        <script type="text/javascript">
          <!--
            if ($('sim').checked) {
                Element.show('arqinput');
            } else {
                Element.hide('arqinput');
            }
          -->
        </script>
<?php
        if ($conteudo->arquivo instanceof Arquivo && $conteudo->arquivo->getPrimaryKey() !== null) {
            echo "        <tr id=\"arqtitle\">\n"
                ,"          <td style=\"text-align: center; font-weight: bold;\">Ações</td>\n"
                ,"          <td style=\"text-align: center; font-weight: bold;\">Arquivo</td>\n"
                ,"          <td style=\"text-align: center; font-weight: bold;\">Tamanho</td>\n"
                ,"        </tr>\n"
                ,"        <tr id=\"arqlist\">\n"
                ,"          <td style=\"text-align: center;\">\n"
                ,"            <a href=\"javascript:void(0)\" onclick=\"excluirArquivo("  , $conteudo->getPrimaryKey() , ")\"><img style=\"border:none;\" src=\"/imagens/excluir_01.gif\" alt=\"Excluir arquivo\" /></a>\n"
                ,"            <a href=\"brasilpro.php?modulo=principal/escolappp&acao=A&downloadArquivo=true&cppid=" ,$conteudo->getPrimaryKey(), "\"\"><img style=\"border:none;\" src=\"/imagens/salvar.png\" alt=\"Salvar arquivo\" /></a>\n"
                ,"          </td>\n"
                ,"          <td style=\"text-align: left;\">" , $conteudo->arquivo->arqnome    , "</td>\n"
                ,"          <td style=\"text-align: center;\">" , $conteudo->arquivo->arqtamanho , " bytes</td>\n"
                ,"        </tr>\n";
        }
    } // !elseif ($_REQUEST['ipptiporesposta'] == 1)
    elseif ($_REQUEST['ipptiporesposta'] == 2) {
        $value_mn = false;
        $value_uf = false;
        $itrid    = cte_pegarItrid($_SESSION['inuid']);

        if (trim($item->ipptitulo) == '18. Índice de Desenvolvimento Humano (IDH) do município em que se localiza a Escola / UF') {
            $entidade = new Entidade($_REQUEST['entid']);
            $entidade->carregarEnderecos();

            $endereco = $entidade->enderecos[0];

            if ($endereco instanceof Endereco) {
                $muncod = $endereco->muncod;
                $estuf  = $endereco->estuf;
            } else {
                $muncod = cte_pegarMuncod($_SESSION['inuid']);
                $estuf  = cte_pegarEstuf($_SESSION['inuid']);
            }

            $value_mn = $db->pegaUm('SELECT idhvalor FROM cte.indiceidh WHERE mun_estuf = \'' . $estuf . '\' and muncod = \'' . $muncod . '\'');
            $value_uf = $db->pegaUm('SELECT idhvalor FROM cte.indiceidh WHERE     estuf = \'' . $estuf . '\'');
        }

        $cpptexto = explode("|", $conteudo->cpptexto);
        if ($value_uf !== false)
            $value_uf .= '" readonly="readonly"';
        else
            $value_uf  = $cpptexto[0];

        if ($value_mn !== false)
            $value_mn .= '" readonly="readonly"';
        else
            $value_mn  = $cpptexto[1];
?>
          <td colspan="3">
            <table width="100%">
              <tr>
                <td align="right" class="SubTituloDireita" style="width:125px">Estado:</td>
                <td>
                  <input type="text" class="CampoEstilo" name="cpptexto[]" size="5" value="<?php echo $value_uf ?>" <? echo (($bloqueiagravacao)?'disabled':''); ?>/>
                  <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                </td>
              </tr>
              <tr>
                <td align="right" class="SubTituloDireita" style="width:125px">Município:</td>
                <td>
                  <input type="text" class="CampoEstilo" name="cpptexto[]" size="5" value="<?php echo $value_mn ?>" <? echo (($bloqueiagravacao)?'disabled':''); ?>/>
                  <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                </td>
              </tr>
            </table>
            <input type="hidden" name="ipptiporesposta" id="ipptiporesposta" value="2" />
          </td>
<?php
    } // !elseif ($_REQUEST['ipptiporesposta'] == 2)
    elseif ($_REQUEST['ipptiporesposta'] == 3) {
        $value_mn = false;
        $value_uf = false;
        $itrid    = cte_pegarItrid($_SESSION['inuid']);

        $entidade = new Entidade($_REQUEST['entid']);
        $entidade->carregarEnderecos();

        $detalhe  = new EntidadeDetalhe($entidade->entcodent);

        $endereco = $entidade->enderecos[0];

        if ($endereco instanceof Endereco) {
            $muncod = $endereco->muncod;
            $estuf  = $endereco->estuf;
        } else {
            $muncod = cte_pegarMuncod($_SESSION['inuid']);
            $estuf  = cte_pegarEstuf($_SESSION['inuid']);
        }

        if (trim($item->ipptitulo) == '19. Média da escola no ENEM / UF') {
            $sql = 'SELECT
                        COUNT(*) AS divisor,
                        SUM(entdnum_enem_media_total_2006) AS soma
                    FROM
                        entidade.entidadedetalhe ed
                    INNER JOIN
                        entidade.entidade ee ON ed.entcodent = ee.entcodent
                    INNER JOIN
                        entidade.endereco ende ON ee.entid = ende.entid AND ende.estuf = \'' . $estuf . '\'
                    WHERE
                        entdnum_enem_media_total_2006 IS NOT NULL';
ver($sql);
            $res      = $db->pegaLinha($sql);

            $value_uf = $res['soma'] / $res['divisor'];
            $value_es = $detalhe->entdnum_enem_media_total_2006;

        } elseif (trim($item->ipptitulo) == '20. Taxa de abandono do Ensino Médio da escola / UF') {
            $sql = 'SELECT
                        COUNT(*) AS divisor,
                        SUM(entdnum_medio_abandono_2005) AS soma
                    FROM
                        entidade.entidadedetalhe ed
                    INNER JOIN
                        entidade.entidade ee ON ed.entcodent = ee.entcodent
                    INNER JOIN
                        entidade.endereco ende ON ee.entid = ende.entid AND ende.estuf = \'' . $estuf . '\'
                    WHERE
                        entdnum_medio_abandono_2005 IS NOT NULL';

            $res      = $db->pegaLinha($sql);

            $value_uf = $res['soma'] / $res['divisor'];
            $value_es = $detalhe->entdnum_medio_abandono_2005;
        }

        $cpptexto = explode("|", $conteudo->cpptexto);

        if ($value_es === null)
            $value_es  = $cpptexto[0];

        if ($value_uf !== false)
            $value_uf .= '" readonly="readonly"';
        else
            $value_uf  = $cpptexto[1];

?>
          <td colspan="3">
            <table width="100%">
              <tr>
                <td align='right' class="SubTituloDireita" style="width:125px">Escola:</td>
                <td>
                  <input type="text" class="CampoEstilo" maxlength="5" name="cpptexto[]" size="5" value="<?php echo $value_es; ?>" <? echo (($bloqueiagravacao)?'disabled':''); ?> maxlength="6"/> 
                  <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                </td>
              </tr>
              <tr>
                <td align='right' class="SubTituloDireita" style="width:125px">Estado:</td>
                <td>
                  <input type="text" class="CampoEstilo" maxlength="5" name="cpptexto[]" size="5" value="<?php echo $value_uf; ?>" <? echo (($bloqueiagravacao)?'disabled':''); ?> maxlength="6"/>
                  <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
                </td>
              </tr>
            </table>
            <input type="hidden" name="ipptiporesposta" id="ipptiporesposta" value="3" />
          </td>

<?php
    } // !elseif ($_REQUEST['ipptiporesposta'] == 2)
    elseif ($_REQUEST['ipptiporesposta'] == 4) {

?>
          <td colspan="3">
            <input type="radio" name="cpptexto" value="sim" id="sim" <?php echo ($cpptexto == 'sim' ? 'checked="checked"' : ''); echo (($bloqueiagravacao)?'disabled':''); ?> /><label for="sim">Sim</label>
            <input type="radio" name="cpptexto" value="nao" id="nao" <?php echo ($cpptexto == 'nao' ? 'checked="checked"' : ''); echo (($bloqueiagravacao)?'disabled':''); ?> /><label for="nao">Não</label>
            <input type="hidden" name="ipptiporesposta" id="ipptiporesposta" value="4" />
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
                  <select name="areacurso" id="areacurso" multiple="true" onclick="return carregarCursoPorArea();" ondblclick="exibirDetalhesArea();" style="height: 30ex; width: 200px;" <? echo (($bloqueiagravacao)?'disabled':''); ?>>
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
                  <select name="cursotecnico" id="cursotecnico" multiple="true" style="height: 30ex; width: 290px;" ondblclick="exibirDetalhesCurso();" <? echo (($bloqueiagravacao)?'disabled':''); ?>>
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
                  <select name="cpptexto[]" id="cpptexto" multiple="true" style="height: 20ex; width: 496px;" <? echo (($bloqueiagravacao)?'disabled':''); ?>>
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
            <input type="hidden" name="ipptiporesposta" id="ipptiporesposta" value="5" />
          </td>

<?php
    } // !elseif ($_REQUEST['ipptiporesposta'] == 5)

?>
        <tr>
          <td style="padding: 5px; background-color:#fafafa; text-align: center" width="20%">
          <?php
          $nItemPPP = $item->anterior();
          if ($nItemPPP !== -1) {
              echo '<a href="brasilpro.php?modulo=principal/escolappp&acao=A&ipptiporesposta=' ,$nItemPPP->ipptiporesposta, '&entid=' , $_REQUEST['entid'] , '&ippid=' , $nItemPPP->ippid , '"><< Anterior</a>';
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

              <input type="submit" value="Salvar" <? echo (($bloqueiagravacao)?'disabled':''); ?> />
              <input type="button" value="Fechar" onclick="return fecharPoUp();" />
          </td>

          <td style="padding: 5px; background-color:#fafafa; text-align: center" width="20%">
          <?php
          $nItemPPP = $item->proximo();
          if ($nItemPPP !== -1) {
              echo '<a href="brasilpro.php?modulo=principal/escolappp&acao=A&ipptiporesposta=' ,$nItemPPP->ipptiporesposta, '&entid=' , $_REQUEST['entid'] , '&ippid=' , $nItemPPP->ippid , '">Próxima >></a>';
          } else {
              echo 'Próxima >>';
          }
          ?>
          </td>
        </tr>
      </table>
    </form>
  </body>

  <script>
    this._closeWindows = false;

    function fecharPoUp()
    {
        window.opener.location.reload();
        window.close();
        window.opener.focus();

        return false;
    }

    Element.hide('aguarde');
  </script>
</html>

