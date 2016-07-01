<?php 
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    require_once APPRAIZ . "gestaodocumentos/classes/Anexo.class.inc";
    require_once APPRAIZ . "gestaodocumentos/classes/GestaoDocumentos.class.inc";

    if($_GET['tarid']){
        # Verifica se existe tarefa
        boExisteTarefa( $_GET['tarid'], true );
        $_SESSION['dados_tarefa']['tarid'] = $_GET['tarid'];	
    }

    if($_GET['download'] == 'S'){
        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($_GET['arqid']);
        echo"<script>window.location.href = 'gestaodocumentos.php?modulo=principal/cadDocumento&acao=A';</script>";
        exit;
    }

    if($_FILES["arquivo"] && !$_POST['arqid']){
        
        $tarid = $_SESSION['dados_tarefa']['tarid'];
                
        $campos	= array("tarid"            => $tarid,
                        "anxdesc"		   => "'{$_POST['anxdesc']}'",
                        "tpdid" 		   => "'{$_POST['tpdid']}'",
                        "anxassunto" 	   => "'{$_POST['anxassunto']}'",
                        "anxnumsidoc" 	   => "'{$_POST['anxnumsidoc']}'",
                        "anxnumdoc" 	   => "'{$_POST['anxnumdoc']}'",
                        "anxprocessosidoc" => "'{$_POST['anxprocessosidoc']}'"
                        );

        $file = new FilesSimec("anexo", $campos ,"gestaodocumentos");
        $arquivoSalvo = $file->setUpload();
        
        if($arquivoSalvo){
            echo '<script type="text/javascript"> 
                    alert("Operação realizada com sucesso!");
                    window.location.href="gestaodocumentos.php?modulo=principal/cadDocumento&acao=A";
                  </script>';
            die;
        }
    } elseif($_POST['arqid'] && $_POST['anxid']) {
        $obAnexo = new Anexo();
        $arCampos = array('anxid','tarid','anxdesc','tpdid','anxassunto','anxnumsidoc','anxnumdoc','anxprocessosidoc');
        $obAnexo->popularObjeto($arCampos);
        $obAnexo->salvar();
        $obAnexo->commit();


        $_REQUEST['acao'] = 'A';
        $db->sucesso("principal/cadDocumento",'');
        unset($obAnexo);
        die;
    }
 
    if($_GET['arqidDel']){
        $sql = "DELETE FROM gestaodocumentos.anexo where arqid=".$_REQUEST['arqidDel'];
        $db->executar($sql);
        $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
        $db->executar($sql);
        $db->commit();
        echo '<script type="text/javascript"> 
                alert("Operação realizada com sucesso!");
                window.location.href="gestaodocumentos.php?modulo=principal/cadDocumento&acao=A";
              </script>';
        die;
    }

    #Chamada de programa
    include  APPRAIZ."includes/cabecalho.inc";
    echo'<br>';
    $db->cria_aba( $abacod_tela, $url, '' );
    monta_titulo( $titulo_modulo, '' );

    #Correçao por Alexandre Dourado
    if(!$_SESSION['dados_tarefa']['tarid']) {
        die("
            <script>
                alert('Tarefa não encontrada. Refaça o procedimento.');
                window.location='gestaodocumentos.php?modulo=principal/listaTarefas&acao=A';
            <script>
        ");
    }

    $tarid = $_SESSION['dados_tarefa']['tarid'];
    $obTarefa  = new GestaoDocumentos();
    $tarTarefa = $obTarefa->pegaTartarefaPorTarid($tarid);

    $obAnexo = new Anexo($_GET['anxid']);

    #CABECALHO
    echo cabecalhoTarefa($tarid);
?>

<br>
<center>
    <div id="aguarde_" style="display: none;position:absolute;color:#000033;top:50%;left:35%; width:300;font-size:12px;z-index:0;">
        <br><img src="../imagens/carregando.gif" border="0" align="middle"><br>Carregando...<br>
    </div>
</center>

<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="./js/gestaodocumentos.js"></script>

<form name=formulario id=formulario method=post enctype="multipart/form-data">
    <input type="hidden" name="anxid" id="anxid" value="<?php echo $obAnexo->anxid; ?>" />
    <input type="hidden" name="arqid" id="arqid" value="<?php echo $obAnexo->arqid; ?>" />
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
            <td width='50%'>
                <input type="file" name="arquivo" id="arquivo"/>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
            </td>      
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo de Documento:</td>
            <td>
                <?php
                $tpdid = $obAnexo->tpdid;
                $sql = "SELECT 	   tpdid as codigo, tpddescricao as descricao
	                	FROM	   gestaodocumentos.tipodocumento
	                	ORDER BY   tpddescricao";
                $db->monta_combo('tpdid', $sql, 'S', 'Selecione...', '', '', '', '290', 'S', 'tpdid');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
            <td>
                <?php
                $anxdesc = $obAnexo->anxdesc;
                echo campo_textarea('anxdesc', 'N', 'S', 'Descrição ', 59, 10, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição', $anxdesc);
                ?>
            </td>
        </tr>
        <tr style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
            <td height="30"><input type="button" name="botao" value="Salvar" onclick="validaForm();"></td>
        </tr> 
    </table>

    <div id="div_listaDocumento"></div>

    <br>

    <?PHP
        echo "
            <script type=\"text/javascript\">
                montaListaAnexo('$tarTarefa');
            </script>
        "; 
    ?>

    <script language="javascript" type="text/javascript">
        if($('anxid').value != ''){
            $('arquivo').disabled = true;
        }

        function validaForm(){
            var alerta = "";
            if($('arquivo').value == '' && $('arqid').value == ''){
                alerta += "Você deve escolher um arquivo.\n";
            }

            if($('tpdid').value == ''){
                alerta += "O campo \"Tipo de Documento\" é Obrigatório.\n";
            }

            if(alerta){
                alert(alerta);
                return false;
            } 
            $('formulario').submit();
        }

        function excluirAnexo( arqid ){
            if ( confirm( 'Deseja excluir o Documento?' ) ) {
                location.href= window.location+'&arqidDel='+arqid;
            }
        }

        function abrirAnexo( arqid ){
            if(arqid){
                window.location='?modulo=principal/cadDocumento&acao=A&download=S&arqid='+arqid;
            }
        }

        function verificaSidoc( codSidoc ){
            var codSidoc = trim(codSidoc);
            codSidoc = codSidoc.replace("-","");
            codSidoc = codSidoc.replace("/",""); 
            if(codSidoc){ 
                var data = 'tipo=pesquisaSidoc&codSidoc='+codSidoc;
                new Ajax.Request('ajax.php',
                {  
                    method: 'post',   
                    parameters: data,
                    onLoading: $('aguarde_').show(),
                    asynchronous: false,
                    onComplete: function(r)
                    {
                        $('divTeste').update(r.responseText);
                        //alert( r.responseText );
                        var obSidoc = r.responseText.evalJSON();

                        if(obSidoc.NumeroAnexador){ 
                            $('anxprocessosidoc').value = mascaraglobal('[#].######/####-##', obSidoc.NumeroAnexador );  
                        } else {
                            $('anxprocessosidoc').value = '';
                        }
                    }
                });
                $('aguarde_').hide()
            }
        }
    </script>

    <div id='divTeste'></div>