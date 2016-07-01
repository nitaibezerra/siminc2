<?php

include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

ini_set("memory_limit", "2048M");
ini_set("upload_max_filesize", "200M");
ini_set("post_max_size", "200M");
set_time_limit(0);

$db = new cls_banco();

require_once("class/ClassImage.php");
$upload = new ClassImage();

if (!is_dir(APPRAIZ . 'arquivos/obras2')) {
    mkdir(APPRAIZ . 'arquivos/obras2', 0777);
}
if (!is_dir(APPRAIZ . 'arquivos/obras2/imgs_tmp')) {
    mkdir(APPRAIZ . 'arquivos/obras2/imgs_tmp', 0777);
}

$storage = "../../../arquivos/obras2/imgs_tmp";
$controletextarea = 0;
for ($i = 0; $i < count($_FILES['Filedata']['name']); $i++) {
    $extensaopermitida = true;
    switch (strtolower((end(explode(".", $_FILES['Filedata']['name'][$i]))))) {
        case 'gif':
            $_FILES['Filedata']['type'][$i] = 'image/gif';
            break;
        case 'jpg':
        case 'jpeg':
            $_FILES['Filedata']['type'][$i] = 'image/jpeg';
            break;
        case 'png':
            $_FILES['Filedata']['type'][$i] = 'image/png';
            break;
        case 'bmp':
            $_FILES['Filedata']['type'][$i] = 'image/bmp';
            break;
        default:
            $extensaopermitida = false;
    }
    if ($extensaopermitida) {
        $foto_name = str_replace("/", "___", md5_encrypt(tirar_acentos(substr($_FILES['Filedata']['name'][$i], 0, 20))) . "__extension__" . md5_encrypt($_FILES['Filedata']['type'][$i]) . "__temp__" . date('YmdHis') . rand(1, 10000));
        $uploadfile = "$storage/$foto_name";
        $uploaded = $upload->reduz_imagem($_FILES['Filedata']['tmp_name'][$i], 640, 480, $uploadfile, $ext = strtolower((end(explode(".", $_FILES['Filedata']['name'][$i])))));

        if (!$uploaded) {
            echo "<script>
                    alert('Problemas na gravação do arquivo.');
                    window.close();
                  </script>";
            exit;
        }

        if ($_REQUEST['funcao'] == 'AtualizaFotos') {

            $valid1 = true;
            $valid2 = true;

            if (!$_SESSION['obras2']['obrid'] || !$_SESSION['obras2']['emiid']) { $valid1 = false; }

            if (!$_SESSION['obras2']['obrid'] || !$_SESSION['obras2']['supid']) { $valid2 = false; }

            if (!$valid1 && !$valid2) {
                die("<script>
                        alert('Problemas com váriaveis de sistema');
                        window.close();
                     </script>");
            }


            if (file_exists("../../../arquivos/obras2/imgs_tmp/" . $foto_name)) {
                
                $foto_name   = str_replace("___", "/", $foto_name);
                $part1file   = explode("__temp__", $foto_name);
                $part2file   = explode("__extension__", $part1file[0]);
                $part2file   = explode("__extension__", is_array($part2file) ? current($part2file) : '');
                $nomearquivo = explode(".", $_FILES['Filedata']['name'][$i]);

                //Insere o registro da imagem na tabela public.arquivo
                $nomearquivo[0] = addslashes($nomearquivo[0]);
                $sql = "INSERT INTO public.arquivo(arqnome,arqdescricao,arqextensao,arqtipo,arqdata,arqhora,usucpf,sisid)
						VALUES('" . $nomearquivo[0] . "','" . $descricao . "','" . $nomearquivo[(count($nomearquivo) - 1)] . "','" . $_FILES['Filedata']['type'][$i] . "','" . date('Y-m-d') . "','" . date('H:i:s') . "','" . $_SESSION["usucpf"] . "',15) RETURNING arqid;";
                $arqid = $db->pegaUm($sql);
                if (!is_dir('../../../arquivos/obras2/' . floor($arqid / 1000))) {
                    mkdir(APPRAIZ . '/arquivos/obras2/' . floor($arqid / 1000), 0777);
                }
                
                if (@copy("../../../arquivos/obras2/imgs_tmp/" . $foto_name, "../../../arquivos/obras2/" . floor($arqid / 1000) . "/" . $arqid)) {

                   if(isset($_SESSION['obras2']['supid'])){
                       $sql = "SELECT fotordem 
                               FROM obras2.fotos 
                               WHERE obrid = '" . $_SESSION['obras2']['obrid'] . "' 
                                 AND supid = '" . $_SESSION['obras2']['supid'] . "' 
                               ORDER BY fotordem DESC LIMIT 1";
                       $ordem = $db->pegaUm($sql);
                       $_sql = "INSERT INTO obras2.fotos(arqid,obrid,supid,fotdsc,fotbox,fotordem)
                                VALUES(" . $arqid . "," 
                                         . $_SESSION['obras2']['obrid'] . "," 
                                         . $_SESSION['obras2']['supid'] . ",'" 
                                         . $foto_name . "','imageBox" . (($ordem) ? ($ordem + 1) : '0') . "'," 
                                         . (($ordem) ? ($ordem + 1) : '0') . ");";
                    }
                    
                   if(isset($_SESSION['obras2']['emiid'])){
                       $sql = "SELECT fotordem 
                               FROM obras2.fotos 
                               WHERE obrid = '" . $_SESSION['obras2']['obrid'] . "' 
                                 AND emiid = '" . $_SESSION['obras2']['emiid'] . "' 
                               ORDER BY fotordem DESC LIMIT 1";
                       $ordem = $db->pegaUm($sql);
                       $_sql = "INSERT INTO obras2.fotos(arqid,obrid,emiid,fotdsc,fotbox,fotordem)
                                VALUES(" . $arqid . "," 
                                         . $_SESSION['obras2']['obrid'] . "," 
                                         . $_SESSION['obras2']['emiid'] . ",'" 
                                         . $foto_name . "','imageBox" . (($ordem) ? ($ordem + 1) : '0') . "'," 
                                         . (($ordem) ? ($ordem + 1) : '0') . ");";
                    }
                    
                    $db->executar($_sql);
                    $db->commit();
                    unlink("../../../arquivos/obras2/imgs_tmp/" . $foto_name);
                } else {
                    $db->rollback();
                    echo "Falha ao copiar o arquivo";
                }
                $op = montaSelectItens($arqid, 0);
                echo "<script>
                            var tabela = parent.document.getElementById('listaimagens');
                            var linha  = tabela.insertRow((tabela.rows.length-1));
                            linha.id='" . $arqid . "';
                            var celulaordenacao = linha.insertCell(0);
                            //celulaordenacao.innerHTML = '<img id=\"setasubir\" src=\"../../imagens/seta_cima.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,\'subir\');\"> <img id=\"setadescer\" src=\"../../imagens/seta_baixo.gif\"  style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,\'descer\');\">';
                            var celulaimg = linha.insertCell(1);
                            celulaimg.innerHTML = \"<img src='../../slideshow/slideshow/verimagem.php?arqid=" . $arqid . "' width=100 height=100>\";
                            var celuladsc = linha.insertCell(2);
                            celuladsc.innerHTML = \"{$op}<textarea  id='supobs' name='supobs[" . $arqid . "]' cols='40' rows='5' onmouseover='MouseOver( this );' onfocus='MouseClick( this );'  onmouseout='MouseOut( this );'  onblur='MouseBlur( this );'  onblur='MouseBlur( this );' onkeydown='textCounter( this.form.supobs[" . $controletextarea . "], this.form.no_supobs" . $controletextarea . ", 255 );'  onkeyup='textCounter( this.form.supobs[" . $controletextarea . "], this.form.no_supobs" . $controletextarea . ", 255);'></textarea><br><input readonly style='text-align:right;border-left:#888888 3px solid;color:#808080;' type='text' name='no_supobs" . $controletextarea . "' size='6' maxlength='6' value='255' class='CampoEstilo'><font color='red' size='1' face='Verdana'> máximo de caracteres</font>\";
                            var celuladeleta = linha.insertCell(3);
                            celuladeleta.innerHTML = '<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirfotosvistoria(this.parentNode.parentNode.rowIndex);\">';
                            parent.inserirfotosthumbnails('" . $arqid . "');
                      </script>";
                $controletextarea++;
            }
        } else {
            $op = montaSelectItens($foto_name, 0);

            echo "<script>
                    var tabela = parent.document.getElementById('listaimagens');
                    var linha  = tabela.insertRow((tabela.rows.length-1));
                    linha.id='" . $foto_name . "';
                    var celulaordenacao = linha.insertCell(0);
                    //celulaordenacao.innerHTML = '<img id=\"setasubir\" src=\"../../imagens/seta_cima.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,\'subir\');\"> <img id=\"setadescer\" src=\"../../imagens/seta_baixo.gif\" style=\"cursor:pointer;\" onclick=\"ordenar(this.parentNode.parentNode.rowIndex,\'descer\');\">';
                    var celulaimg = linha.insertCell(1);
                    celulaimg.innerHTML = \"<img src='resize.php?img=../../../arquivos/obras2/imgs_tmp/" . $foto_name . "&w=100&h=100'>\";
                    var celuladsc = linha.insertCell(2);
                    celuladsc.innerHTML = \"{$op}<textarea  id='supobs' name='supobs[" . $foto_name . "]' cols='40' rows='5' onmouseover='MouseOver( this );' onfocus='MouseClick( this );'  onmouseout='MouseOut( this );'  onblur='MouseBlur( this );' onkeydown='textCounter( this.form.supobs[" . $controletextarea . "], this.form.no_supobs" . $controletextarea . ", 255 );'  onkeyup='textCounter( this.form.supobs[" . $controletextarea . "], this.form.no_supobs" . $controletextarea . ", 255);'></textarea><br><input readonly style='text-align:right;border-left:#888888 3px solid;color:#808080;' type='text' name='no_supobs" . $controletextarea . "' size='6' maxlength='6' value='255' class='CampoEstilo'><font color='red' size='1' face='Verdana'> máximo de caracteres</font>\";
                    var celuladeleta = linha.insertCell(3);
                    celuladeleta.innerHTML = '<img id=\"excluirfotovistoria\" src=\"../../imagens/excluir.gif\" style=\"cursor:pointer\" onclick=\"excluirfotosvistoria(this.parentNode.parentNode.rowIndex);\">';
                    parent.inserirfotosthumbnailsPorNome('" . $foto_name . "');
                    parent.document.getElementById('submit1').disabled = false;
                    parent.document.getElementById('submit2').disabled = false;
                 </script>";
            $controletextarea++;
        }
    }
}
echo "<script>
        var formanexo = parent.document.getElementById('anexo');
        for(i=0;i<formanexo.elements.length;i++) {
            if(formanexo.elements[i].type == \"file\") {
                formanexo.elements[i].value = \"\";
            }
        }
        window.close();
      </script>";
?>





<?
function montaSelectItens($arqid, $default)
{
    $itens = pegaItensCronograma();
    $op =  "<select name='icoid[".$arqid."]'>";
    foreach ($itens as $item) {
        $selected = ($item['icoid'] == $default) ? 'selected=\'selected\'' : '';
        $op .= "<option $selected value='{$item['icoid']}'>{$item['itcdesc']}</option>";
    }
    $op .=  "</select><br />";

    return $op;
}

function pegaItensCronograma()
{
    global $db;

	$obrid = $_SESSION['obras2']['obrid'];

	if( empty($obrid ) )
		return array(array('icoid'=>'0', 'itcdesc'=>' -- Selecione a Etapa -- '));

    $sql = "SELECT
                    i.icoid,
                    io.itcdesc
                FROM obras2.cronograma c
                JOIN obras2.itenscomposicaoobra i ON i.croid = c.croid AND i.obrid = c.obrid
                JOIN obras2.itenscomposicao io ON io.itcid = i.itcid
                WHERE c.crostatus = 'A' AND c.obrid = {$_SESSION['obras2']['obrid']}
                ORDER BY i.icoordem";
    $itens = $db->carregar($sql);
    $itens = (empty($itens)) ? array() : $itens;
    return array_merge( array(array('icoid'=>'0', 'itcdesc'=>' -- Selecione a Etapa -- ')), $itens);
}
?>