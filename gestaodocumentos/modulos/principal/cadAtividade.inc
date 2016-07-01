<?PHP
    require_once APPRAIZ . "gestaodocumentos/classes/GestaoDocumentos.class.inc";
    require_once APPRAIZ . "includes/classes/dateTime.inc";

    # Verifica se existe tarefa
    if($_GET['tarid']){
        boExisteTarefa( $_GET['tarid'], true );
    }

    if($_POST['acaoForm']){
        extract($_POST);
        $obAtividade = new GestaoDocumentos();

        $arCampos = array('tarid','_tarpai','_tartarefa','tartitulo','tarprioridade','tardsc','unaidsetororigem','unaidsetorresponsavel','sitid','tardepexterna');
        $obAtividade->popularObjeto($arCampos);
        $obAtividade->tartipo		 		  = "A";

        $obData = new Data();
        if($tardatainicio){
            $obAtividade->tardatainicio = $obData->formataData($tardatainicio,"YYYY-mm-dd");    
        }else{
            $obAtividade->tardatainicio = NULL;
        }
        if($tardataprazoatendimento){
            $obAtividade->tardataprazoatendimento = $obData->formataData($tardataprazoatendimento,"YYYY-mm-dd");
        }else{
            $obAtividade->tardataprazoatendimento = NULL;
        }
        if($tardataprazoatendimento){
            $obAtividade->tardataprazolinhadebase = $obData->formataData($tardataprazoatendimento,"YYYY-mm-dd");
        }else{
            $obAtividade->tardataprazolinhadebase = NULL;
        }

        if(!$obAtividade->tarid && $_tarpai){

            $sql = "select 
                        max(_tarordem)
                    from gestaodocumentos.tarefa where _tarpai = $_tarpai";

            $max_tarordem = $db->pegaUm($sql);
            if($max_tarordem){
                $tamanho = strlen($max_tarordem);
                $tamanho = ($tamanho) ? $tamanho : 4;

                $substr = substr($max_tarordem, 0, $tamanho - 4);
                $substr2 = substr($max_tarordem, $tamanho -4, 4);
                $soma = intval($substr2) + 1;
                $soma = str_pad($soma, 4, 0, STR_PAD_LEFT);

                $max_ordem = $substr . $soma;
            } else {
                $sql = "select 
                            max(_tarordem)
                        from gestaodocumentos.tarefa where tarid = $_tarpai";

                $max_tarordem = $db->pegaUm($sql);
                $max_ordem = $max_tarordem . '0001';
            }
            $obAtividade->_tarordem = $max_ordem;
        }

        $obAtividade->usucpfresponsavel = ($usucpfresponsavel) ? $usucpfresponsavel : null;
        $arCamposNulo = array('usucpfresponsavel');

        $obAtividade->salvar(false, false, $arCamposNulo);
        if(trim($acodsc)){
            $obAtividade->salvarAcompanhamento($_POST, true);
        }
        $obAtividade->commit();

        $db->sucesso("principal/cadAcompanhamento","&tarid=".$obAtividade->tarid);
        unset($obAtividade);
        die;
    }
    if($_GET['taridExcluir']){
        $obTarefa = new GestaoDocumentos();
        $obTarefa->excluir($_GET['taridExcluir']);
        $obTarefa->commit();
        unset($_GET['taridExcluir']);
        unset($obTarefa);
        $db->sucesso("principal/cadAtividade");
        die;
    }

    if($_GET['tarpai']){
        $obTarefa = new GestaoDocumentos($_GET['tarpai']);
        $_SESSION['_tartarefa'] = $_GET['tartarefa'];
        $_SESSION['dados_tarefa']['tarid'] = $_GET['tarpai'];
    } elseif($_SESSION['dados_tarefa']['tarid']){
        $obTarefa = new GestaoDocumentos($_SESSION['dados_tarefa']['tarid']);
    } elseif($_SESSION['tarid'] && !$_GET['tartarefa']){
        $obTarefa = new GestaoDocumentos($_SESSION['tarid']);
    } elseif(!$_GET['tarpai']) {
        $_GET['tarpai'] = $_GET['tartarefa'];
        $obTarefa = new GestaoDocumentos($_GET['tarpai']);
        $_SESSION['tarid'] = $_GET['tarpai'];
    }
    $_SESSION['tarid'] = $_GET['tarid'];
    $obAtividade = new GestaoDocumentos($_GET['tarid']);

    //Chamada de programa
    include  APPRAIZ."includes/cabecalho.inc";
    echo'<br>';
    $db->cria_aba( $abacod_tela, $url, '' );
    monta_titulo( $titulo_modulo, '' );
    $habilitado = $_SESSION['gt_doc']['perfilHabilitado'];
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>

<script type="text/javascript" src="./js/gestaodocumentos.js" ></script>
<script type="text/javascript" src="./js/ajax.js"></script>

<center>
    <div id="aguarde_" style="display: none;position:absolute;color:#000033;top:50%;left:35%; width:300;font-size:12px;z-index:0;">
        <br><img src="../imagens/carregando.gif" border="0" align="middle"><br>Carregando...<br>
    </div>
</center>

<form method="post" name="formulario" id="formulario" action="/gestaodocumentos/gestaodocumentos.php?modulo=principal/cadAtividade&acao=A">
    <input type="hidden" name="acaoForm" id="acaoForm" value="1" />
    <input type="hidden" name="cadAtividade" id="cadAtividade" value="1" />
    <input type="hidden" name="_tarpai" id="_tarpai" value="<? echo $obTarefa->tarid; ?>" />
    <input type="hidden" name="_tartarefa" id="_tartarefa" value="<? echo $obTarefa->_tartarefa; ?>" />
    <input type="hidden" name="tardataprazoatendimentoTarefa" id="tardataprazoatendimentoTarefa" value="<? echo $_SESSION['dados_tarefa']['tardataprazoatendimento']; ?>" />
    <input type="hidden" name="tarid" id="tarid" value="<? echo $obAtividade->tarid; ?>" />
    <!-- CABECALHO -->
    <?PHP
        echo cabecalhoTarefa($obTarefa->tarid);
    ?>
    <!-- DADOS DO ATIVIDADE -->
    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td align="left" colspan="2"><b>DADOS DA ATIVIDADE</b></td>
        </tr>
        <tr>
            <td class="SubTituloDireita" >
                Título:
            </td>
            <td><? $tartitulo = $obAtividade->tartitulo; ?>
                <?= campo_texto('tartitulo', 'S', $habilitado, 'Título', 71, 255, '', '', '', '', '', 'id="tartitulo"'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top">
                Descrição:
            </td>
            <td><? $tardsc = $obAtividade->tardsc; ?>
                <?= campo_textarea('tardsc', 'N', 'S', 'Descrição ', 104, 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Descrição', $tardsc); ?>
            </td>
        </tr>
        <!-- DADOS SOLICITANTES -->
        <?php
        echo blocoDadosAtendimento($obAtividade, 'N', false, false, '', array(), true);
        ?>
    </table>
    
    <div id=buttonAcao>
        <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-top:none">
            <tr>
                <td class="SubTituloDireita" colspan="2" style="text-align:center">
                    <input type="button" value="Salvar" onclick="enviaForm();"/> 
                    <input type="button" value="Incluir Novo" onclick="window.location.href='gestaodocumentos.php?modulo=principal/cadAtividade&acao=A';" />
                </td>
            </tr>
        </table>
    </div>
</form>
<script type="text/javascript">
    function enviaForm(){
        var nomeform        = 'formulario';
        var submeterForm    = true;
        var campos          = new Array();
        var tiposDeCampos   = new Array();

        campos[0] = "tartitulo";
        campos[1] = "unaidsetororigem";
        campos[2] = "unaidsetorresponsavel";
        campos[3] = "tardatainicio";
        campos[4] = "tardataprazoatendimento";
        campos[5] = "sitid";

        tiposDeCampos[0] = "texto";
        tiposDeCampos[1] = "select";
        tiposDeCampos[2] = "select";
        tiposDeCampos[3] = "data";
        tiposDeCampos[4] = "data";
        tiposDeCampos[5] = "select";

        validaForm(nomeform, campos, tiposDeCampos, submeterForm )
    }
	
    function excluirTarefaAtividade(tarid){
        if(confirm('Deseja excluir este registro')){
            window.location.href = "/gestaodocumentos/gestaodocumentos.php?modulo=principal/cadAtividade&acao=A&taridExcluir="+tarid;
            return true;
        } else {
            return false;
        }	
    }
    
</script>