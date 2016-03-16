<?PHP
    include_once APPRAIZ . "gestaopessoa/classes/FtDadoPessoal.class.inc";
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    $bloquearEdicao = bloqueiaEdicaoFT();

    function getImagemServidor() {
        global $db;
        $sql = "
            SELECT *
            FROM gestaopessoa.anexogp AS agp
            INNER JOIN public.arquivo AS arq ON arq.arqid = agp.arqid
            WHERE agp.fdpcpf = '{$_SESSION['fdpcpf']}' AND agp.agpstatus = 'A'
        ";
        $dados = $db->carregar($sql);

        if ($dados) {
            //return $dir = 'file:///'.APPRAIZ.'arquivos/gestaopessoa/'.floor($dados[0]['arqid']/1000).'/'.$dados[0]['arqid'].'.'.$dados[0]['arqextensao'];
            return $dir = $dados[0]['arqid'];
        } else {
            return false;
        }
    }

    if( $_POST['ajaxestuf'] ){
        header('content-type: text/html; charset=ISO-8859-1');
        $sql = "
            SELECT  muncod as codigo,
                    mundescricao as descricao
            FROM territorios.municipio

            WHERE estuf = '{$_POST['ajaxestuf']}'
            ORDER BY mundescricao asc
        ";
        die($db->monta_combo("muncod", $sql, 'S', 'Selecione...', 'filtraEstado(this.value);', '', '', '', 'S', 'muncod'));
    }

    if( $_POST['VerificaUsuarioExixte'] ){
        header('content-type: text/html; charset=ISO-8859-1');

        $cpf = $_POST['VerificaUsuarioExixte'];

        $sql = "
            SELECT * from gestaopessoa.ftdadopessoal where fdpcpf = '{$cpf}'
        ";
        $dados = $db->pegaLinha($sql);
        
        if( $dados['fdpcpf'] != '' ){
            $_SESSION['fdpcpf'] = $dados['fdpcpf'];
            die('SE');
        }else{
            die('NE');            
        }
    }

    if( $_POST['ajaxmuncod'] ){
        header('content-type: text/html; charset=ISO-8859-1');

        $sql = "
            SELECT  estuf
            FROM territorios.municipio
            WHERE muncod = '{$_POST['ajaxmuncod']}'
        ";
        die($db->pegaUm($sql));
    }

    if( $_POST['ajaxremovefoto'] ){
        header('content-type: text/html; charset=ISO-8859-1');

        $sql = "UPDATE public.arquivo SET arqstatus = 'I' WHERE arqid in (SELECT arqid FROM gestaopessoa.anexogp WHERE fdpcpf = '{$_SESSION['fdpcpf']}')";
        $db->executar($sql);

        $sql = "UPDATE gestaopessoa.anexogp SET agpstatus = 'I' WHERE fdpcpf = '{$_SESSION['fdpcpf']}'";
        $db->executar($sql);

        $db->commit();
        echo "OK";
        exit;
    }

    $ft = new FtDadoPessoal();
    if( $_POST['tipo_acao'] == 'GRAVAR' ){
        $ft->fdpcpf                         = str_replace(".", "", str_replace("-", "", $_POST['fdpcpf']));
        $ft->fdpnome                        = $_POST['fdpnome'];
        $ft->fdpsiape                       = $_POST['fdpsiape'];
        $ft->eciid                          = $_POST['eciid'];
        $ft->fdpconjuge                     = $_POST['fdpconjuge'];
        $ft->fdpnomepai                     = $_POST['fdpnomepai'];
        $ft->fdpnomemae                     = $_POST['fdpnomemae'];
        $ft->fdpsexo                        = $_POST['fdpsexo'];
        $ft->fdpdatanascimento              = $_POST['fdpdatanascimento'] ? formata_data_sql($_POST['fdpdatanascimento']) : null;
        $ft->fdpnaturalidade                = $_POST['fdpnaturalidade'];
        $ft->fdpenderecoresidencial         = $_POST['fdpenderecoresidencial'];
        $ft->fdpbairro                      = $_POST['fdpbairro'];
        $ft->muncod                         = $_POST['muncod'];
        $ft->estuf                          = $_POST['estuf'];
        $ft->fdpcep                         = $_POST['fdpcep'];
        $ft->fdpdddtelefoneres              = $_POST['fdpdddtelefoneres'];
        $ft->fdptelefoneres                 = $_POST['fdptelefoneres'];
        $ft->fdpdddcelular                  = $_POST['fdpdddcelular'];
        $ft->fdpcelular                     = $_POST['fdpcelular'];
        $ft->fdpsexo                        = $_POST['fdpsexo'];
        $ft->fdpgruposanguineo              = $_POST['fdpgruposanguineo'];
        $ft->fdpfatorrh                     = $_POST['fdpfatorrh'];
        $ft->fdpcor                         = $_POST['fdpcor'];
        $ft->fdpdeficiente                  = $_POST['fdpdeficiente'];
        $ft->fdpdeficiencia                 = $_POST['fdpdeficiencia'];
        $ft->fdpidentidade                  = $_POST['fdpidentidade'];
        $ft->fdpidexpeditor                 = $_POST['fdpidexpeditor'];
        $ft->fdpufidentidade                = $_POST['fdpufidentidade'];
        $ft->fdpiddataexpedicao             = $_POST['fdpdatanascimento'] ? formata_data_sql($_POST['fdpiddataexpedicao']) : null;
        $ft->fdpregistroprofissional        = $_POST['fdpregistroprofissional'];
        $ft->fdpdataregistroprofissional    = formata_data_sql($_POST['fdpdataregistroprofissional']) ? formata_data_sql($_POST['fdpdataregistroprofissional']) : null;
        $ft->fdptituloeleitor               = $_POST['fdptituloeleitor'];
        $ft->fdpzonaeleitoral               = $_POST['fdpzonaeleitoral'];
        $ft->fdpsecaoeleitoral              = $_POST['fdpsecaoeleitoral'];
        $ft->fdplocaleleitoral              = $_POST['fdplocaleleitoral'];
        $ft->fdpdataprimeiroemprego         = formata_data_sql($_POST['fdpdataprimeiroemprego']) ? formata_data_sql($_POST['fdpdataprimeiroemprego']) : null;
        $ft->fstid                          = $_POST['fstid'];
        
//        $ft->fdpcedido                      = $_POST['fdpcedido'] == '' ? NULL : $_POST['fdpcedido'];
//        $ft->fdpgdap                        = $_POST['fdpgdap'] == '' ? NULL : $_POST['fdpgdap'];
//        $ft->fdpchefiacedido                = $_POST['fdpchefiacedido'] == '' ? NULL : $_POST['fdpchefiacedido'];
        
        $ft->fdporgaochefiaavaliador        = $_POST['fdporgaochefiaavaliador'] == '' ? NULL : $_POST['fdporgaochefiaavaliador'];
        $ft->ftptipogratificacao            = $_POST['ftptipogratificacao'];
        
        $ft->fdpatualizacao                 = $_SESSION['exercicio'];

        $fdpcpf = str_replace(".", "", str_replace("-", "", $_POST['fdpcpf']));
        
        $boExiste = $ft->pegaUm("select fdpcpf from gestaopessoa.ftdadopessoal where fdpcpf = '{$fdpcpf}'");
        
        if ($fdpcpf && $boExiste) {
            $ft->fdpcpf = "'" . str_replace(".", "", str_replace("-", "", $fdpcpf)) . "'";
            $ft->alterar();
        } else {
            $sessionCpf = $ft->inserir();
            $_SESSION['fdpcpf'] = $sessionCpf;
        }
        $ft->commit();

        $campos = array(
            "fdpcpf"            => "'" . $_SESSION['fdpcpf'] . "'",
            "agpdatainclusao"   => "'" . date('Y-m-d') . "'",
            "agpstatus"         => "'A'",
            "usucpf"            => "'" . $_SESSION['usucpf'] . "'"
        );

        $_SESSION["sisdiretorio"] = "gestaopessoa";

        $file = new FilesSimec("anexogp", $campos, "gestaopessoa");
        if ($_FILES['Arquivo']['tmp_name'] != "") {
            $sql = "UPDATE gestaopessoa.anexogp SET agpstatus = 'I' WHERE fdpcpf = '" . $_SESSION['fdpcpf'] . "'";
            $db->executar($sql);
            $db->commit();
            $file->setMover($_FILES['Arquivo']['tmp_name'], "image/jpeg");
        }

        $ft->sucesso("principal/forca_trabalho/cad_dados_pessoais","&opc=".$_POST['tipo_opc']);
        $_SESSION["sisdiretorio"] = "gestaopessoa";
    }

    switch( $_REQUEST['opc'] ){
        case 'FT':
            $titulo = "Força de Trabalho";
        break;
        case 'SC':
            $titulo = "Servidores Cedidos";
        break;
        case 'GD':
            $titulo = "Servidores GDAP";
        break;
        case 'CC':
            $titulo = "Servidores de Chefia Cedidos";
        break;
    }

    if( !isset($_GET['pop']) ){
        include_once APPRAIZ . "includes/cabecalho.inc";
        include_once APPRAIZ . "www/includes/webservice/cpf.php";
        echo '<br>';
    }else{
        include_once( APPRAIZ . "www/includes/webservice/cpf.php" );
    }

    if( $_GET['pop'] == 'sim' ){
        unset($_SESSION['fdpcpf']);
        $_SESSION['fdpcpf'] = trim($_GET['fdpcpf']);

        $nao_visualizar = array(5461, 5542, 5543, 5544, 5545, 6480, 7556);
        $db->cria_aba($abacod_tela, $url, '&pop=sim&fdpcpf='.$_SESSION['fdpcpf'], $nao_visualizar);

        $bloquearEdicao = "disabled=disabled";

        echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>';
        echo '<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';
    }

    $perfil = arrayPerfil();
    if( $_SESSION['fdpcpf'] != '' ){
        $ft->carregarPorId("'{$cpf_servidor}'");
        
        $sql    = "SELECT * FROM gestaopessoa.ftdadopessoal WHERE fdpcpf = '{$_SESSION['fdpcpf']}'";
        $dados  = $db->carregar($sql);
    }

    if( $_SESSION['fdpcpf'] != '' && !isset($_GET['pop']) ){
        $fdpcpf         = $_SESSION['fdpcpf'];
        $habilita_cpf   = "N";
        
        if( $_REQUEST['opc'] != 'CC' ){
            $db->cria_aba($abacod_tela, $url, '');
        }
        
    }elseif( $_SESSION['fdpcpf'] != '' && isset($_GET['pop']) ){
        $fdpcpf         = $_SESSION['fdpcpf'];
        $habilita_cpf   = "N";
    }

    echo '<br>';
    monta_titulo("Força de Trabalho", "<b>{$titulo} - Dados Pessoais</b>");

    unset($_SESSION['sessionFILES']);
    unset($_FILES['Arquivo']);
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>

<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" enctype="multipart/form-data">
    <input type="hidden" id="tipo_acao" name="tipo_acao" value="">
    <input type="hidden" id="tipo_opc" name="tipo_opc" value="<?=$_REQUEST['opc'];?>">

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td colspan="2" class="SubTituloEsquerda"> DADOS PESSOAIS </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right"> CPF: </td>
            <td>
                <?PHP
                    $fdpcpf = formatar_cpf( $fdpcpf );
                    echo campo_texto('fdpcpf', 'S', $habilita_cpf, '', 27, 14, '###.###.###-##', '', '', '', 0, 'id="fdpcpf"', '', $fdpcpf, 'VerificaUsuarioExixte(this.value)', null);
                ?>
            </td>
        </tr>
        <?PHP
            if( $_REQUEST['opc'] != 'CC' ){
        ?>        
                <tr>
                    <td class ="SubTituloDireita">Gratificação:</td>
                    <td>
                        <?PHP
                            switch( $dados[0]['ftptipogratificacao'] ){
                                case 'PE':
                                    $check_PE = 'checked="checked"';
                                break;
                                case 'CE':
                                    $check_CE = 'checked="checked"';
                                break;
                                case 'PS':
                                    $check_PS = 'checked="checked"';
                                break;
                                case 'OS':
                                    $check_OS = 'checked="checked"';
                                break;
                            }
                        ?>
                        <input type="radio" name="ftptipogratificacao" id="ftptipogratificacao_PE" value="PE" <?=$check_PE;?>> <b>GDPGPE</b>
                        <input type="radio" name="ftptipogratificacao" id="ftptipogratificacao_CE" value="CE" <?=$check_CE;?>> <b>GDACE</b>
                        <input type="radio" name="ftptipogratificacao" id="ftptipogratificacao_PS" value="PS" <?=$check_PS;?>> <b>GDAPS</b>
                        <input type="radio" name="ftptipogratificacao" id="ftptipogratificacao_OS" value="OS" <?=$check_OS;?>> <b>CEDIDO</b>
                        <img src="../imagens/obrig.gif" style="margin-top:-10px; margin-left:4px;">
                    </td>
                </tr>
        <?PHP
            }else{
        ?>
                <input type="hidden" name="ftptipogratificacao" id="ftptipogratificacao_FE" value="FE">
        <?PHP
            }
        ?>
        <tr>
            <td class ="SubTituloDireita" align="right">Situação no MEC: </td>
            <td>
                <?PHP
                    $sql = "
                        SELECT fstid AS codigo,
                               fstdescricao AS descricao
                        FROM gestaopessoa.ftsituacaotrabalhador
                    ";
                    if (in_array(PERFIL_SERVIDOR, $perfil) || in_array(PERFIL_TERCEIRIZADO, $perfil) || in_array(PERFIL_CONSULTOR, $perfil)){
                        $sql.= "WHERE fstid in ( " . implode(",", controlaPefilFT('vinculosPermitidos')) . ")";
                    }

                    $fstid = $dados[0]['fstid'];

                    if( $_REQUEST['opc'] == 'CC' ){
                        $fstid = 12;
                    }

                    $_SESSION['sit'] = $fstid;
                    $db->monta_combo('fstid', $sql, 'S', "Selecione...", '', '', '', 200, 'S', 'fstid', '', $fstid, 'Situação no MEC');
                ?>
            </td>
        </tr>
        <?PHP
            if( $_REQUEST['opc'] == 'CC' ){
        ?>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Nome do Orgão Lotado: </td>
                    <td>
                        <?PHP
                            $fdporgaochefiaavaliador = $dados[0]['fdporgaochefiaavaliador'];
                            echo campo_texto('fdporgaochefiaavaliador', 'S', 'S', '', 80, 254, '', '', 'left', '', 0, 'id="fdporgaochefiaavaliador"');
                        ?>
                    </td>
                </tr>
        <?PHP
            }
        ?>
        <tr>
            <td class ="SubTituloDireita" align="right">Nome: </td>
            <td>
                <?PHP
                    $fdpnome = $dados[0]['fdpnome'];
                    echo campo_texto('fdpnome', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '', 0, 'id="fdpnome"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Foto: </td>
            <td height="4cm">
                <?PHP
                    if( !getImagemServidor() ){
                ?>
                        Imagem não cadastrada.
                <?PHP
                    } else {
                ?>
                        <div style="width: 3cm; height: 4cm; border: 8px solid #CDC9C9; text-align: center;">
                            <div style="width: 3cm; position: relative; height: 4cm; border: 4px solid white; width: 100%; left:-4px; top:-3px;">
                                <img width="3cm" height="4cm" src="../slideshow/slideshow/verimagem.php?arqid=<?= getImagemServidor(); ?>&_sisarquivo=gestaopessoa" id="imagem" style="width: 3cm; height: 4cm;">
                            </div>
                        </div>
                        <div style="width: 3cm; text-align: center;">
                            <a href='javascript:RmvFoto()'><b><font color=red>(x) Remover</font></b></a>
                        </div>
                        <br>
                <?PHP
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Imagem 3x4: </td>
            <td>
                <?php
                    if (!isset($_GET['pop'])) {
                        include_once( APPRAIZ . "gestaopessoa/classes/AjaxFileUploader.inc.php" );
                        $ajaxFileUploader = new AjaxFileuploader($uploadDirectory = "");
                        echo $ajaxFileUploader->showFileUploader('Arquivo');
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Matrícula SIAPE: </td>
            <td>
                <?PHP
                    $fdpsiape = $dados[0]['fdpsiape'];
                    echo campo_texto('fdpsiape', 'S', $somenteLeitura, 'Matrícula SIAPE', 26, 7, '#######', '', '', '', 0, 'id="fdpsiape"', '', $fdpsiape, '', null);
                ?>
            </td>
        </tr>
        <?PHP
            if( $_REQUEST['opc'] != 'CC' ){
        ?>
                <tr>
                    <td class ="SubTituloDireita" align="right">Estado Civíl: </td>
                    <td>
                        <?PHP
                            $sql = "
                                SELECT  eciid AS codigo,
                                        ecidescricao AS descricao
                                FROM public.estadocivil
                            ";
                            $eciid = $dados[0]['eciid'];
                            $db->monta_combo('eciid', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'eciid');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Cônjuge: </td>
                    <td>
                        <?PHP
                            $fdpconjuge = $dados[0]['fdpconjuge'];
                            echo campo_texto('fdpconjuge', 'N', $somenteLeitura, '', 80, 200, '', '', 'left', '', 0, 'id="fdpconjuge"');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Nome do Pai: </td>
                    <td>
                        <?PHP
                            $fdpnomepai = $dados[0]['fdpnomepai'];
                            echo campo_texto('fdpnomepai', 'N', $somenteLeitura, '', 80, 200, '', '', 'left', '', 0, 'id="fdpnomepai"');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Nome da Mãe: </td>
                    <td>
                        <?PHP
                            $fdpnomemae = $dados[0]['fdpnomemae'];
                            echo campo_texto('fdpnomemae', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '', 0, 'id="fdpnomemae"');
                        ?>
                    </td>
                </tr>
        <?PHP
            }else{
        ?>
                <input type="hidden" name="eciid" id="eciid" value="2">
                <input type="hidden" name="fdpconjuge" id="fdpconjuge" value="NI">
                <input type="hidden" name="fdpnomepai" id="fdpnomepai" value="NI">
                <input type="hidden" name="fdpnomemae" id="fdpnomemae" value="NI">
        <?PHP
            }
        ?>
        <tr>
            <td class ="SubTituloDireita" align="right">Sexo: </td>
            <td>
                <?PHP
                    switch( $dados[0]['fdpsexo'] ){
                        case 'm':
                            $check_m = 'checked="checked"';
                        break;
                        case 'f':
                            $check_f = 'checked="checked"';
                        break;
                    }
                ?>
                <input type="radio" name="fdpsexo" id="fdpsexo[m]" <?=$check_m;?> value="m" style="margin-left:1px;"> Masculino
                <input type="radio" name="fdpsexo" id="fdpsexo[f]" <?=$check_f;?> value="f" style="margin-left:10px;"> Feminino
            </td>
        </tr>
        <?PHP
            if( $_REQUEST['opc'] != 'CC' ){
        ?>
                <tr>
                    <td class ="SubTituloDireita" align="right">Data de Nascimento: </td>
                    <td>
                        <?PHP
                            $fdpdatanascimento = $dados[0]['fdpdatanascimento'];
                            echo campo_data2('fdpdatanascimento', 'S', 'S', '', 'S');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Naturalidade: </td>
                    <td>
                        <?PHP
                            $fdpnaturalidade = $dados[0]['fdpnaturalidade'];
                            echo campo_texto('fdpnaturalidade', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '', 0, 'id="fdpnaturalidade"');
                        ?>
                    </td>
                </tr>
        <?PHP
            }else{
        ?>
                <input type="hidden" name="fdpdatanascimento" id="fdpdatanascimento" value="01/01/1900">
                <input type="hidden" name="fdpnaturalidade" id="fdpnaturalidade" value="NI">
        <?PHP
            }

            if( $_REQUEST['opc'] != 'CC' ){
                $titulo = 'Endereço Residencial:';
            }else{
                $titulo = 'Endereço do Orgão:';
            }
        ?>
        <tr>
            <td class ="SubTituloDireita" align="right"> <?=$titulo;?> </td>
            <td>
                <?PHP
                    $fdpenderecoresidencial = $dados[0]['fdpenderecoresidencial'];
                    echo campo_texto('fdpenderecoresidencial', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '', 0, 'id="fdpenderecoresidencial"', '', $fdpenderecoresidencial);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Bairro: </td>
            <td>
                <?PHP
                    $fdpbairro = $dados[0]['fdpbairro'];
                    echo campo_texto('fdpbairro', 'S', $somenteLeitura, '', 80, 200, '', '', 'left', '', 0, 'id="fdpbairro"');
                ?>
            </td>
        </tr>

        <tr>
            <td class ="SubTituloDireita" align="right">UF: </td>
            <td id="td_estado">
                <?PHP
                    $estuf = $dados[0]['estuf'];
                    $sql = "
                        SELECT  estuf AS codigo,
	                        estdescricao AS descricao
                        FROM territorios.estado
                        ORDER BY estdescricao
                    ";
                    $db->monta_combo('estuf', $sql, 'S', "Selecione...", 'filtraMunicipio(this.value);', '', '', '200', 'S', 'estuf');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Cidade: </td>
            <td id="td_municipio">
                <?PHP
                    $muncod = $dados[0]['muncod'];
                    $sql = "
                        SELECT  muncod AS codigo,
                                mundescricao AS descricao
                        FROM territorios.municipio
                        ORDER BY mundescricao
                    ";
                    $db->monta_combo('muncod', $sql, 'S', "Selecione...", '', '', '', '200', 'S', 'muncod');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">CEP: </td>
            <td>
                <?PHP
                    $fdpcep = $dados[0]['fdpcep'];
                    echo campo_texto('fdpcep', 'S', $somenteLeitura, '', 10, 8, '########', '', 'left', '', 0, 'id="fdpcep"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Telefone: </td>
            <td>
                <?PHP
                    $fdpdddtelefoneres  = $dados[0]['fdpdddtelefoneres'];
                    $fdptelefoneres     = $dados[0]['fdptelefoneres'];
                    echo '('. campo_texto('fdpdddtelefoneres', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="fdpdddtelefoneres"') .')';
                    echo campo_texto('fdptelefoneres', 'N', $somenteLeitura, '', 11, 9, '#####-####', '', 'left', '', 0, 'id="fdptelefoneres"');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Celular: </td>
            <td>
                <?PHP
                    $fdpdddcelular = $dados[0]['fdpdddcelular'];
                    $fdpcelular = $dados[0]['fdpcelular'];
                    echo '(' .campo_texto('fdpdddcelular', 'N', $somenteLeitura, '', 2, 2, '##', '', 'left', '', 0, 'id="fdpdddcelular"').')';
                    echo campo_texto('fdpcelular', 'N', $somenteLeitura, '', 11, 9, '#####-####', '', 'left', '', 0, 'id="fdpcelular"');
                ?>
            </td>
        </tr>
        <?PHP
            if( $_REQUEST['opc'] != 'CC' ){
        ?>
                <tr>
                    <td colspan="2" class="SubTituloEsquerda"> CARACTERÍSTICAS FÍSICAS </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Grupo Sanguíneo: </td>
                    <td>
                        <select class="CampoEstilo" name="fdpgruposanguineo" id="fdpgruposanguineo" value="<? $dados[0]['fdpgruposanguineo']; ?>">
                            <option value="">Selecione...</option>
                            <option <? if ($dados[0]['fdpgruposanguineo'] == 'A') echo "selected=selected"; ?> value="A">A</option>
                            <option <? if ($dados[0]['fdpgruposanguineo'] == 'B') echo "selected=selected"; ?> value="B">B</option>
                            <option <? if ($dados[0]['fdpgruposanguineo'] == 'AB') echo "selected=selected"; ?> value="AB">AB</option>
                            <option <? if ($dados[0]['fdpgruposanguineo'] == 'O') echo "selected=selected"; ?> value="O">O</option>
                        </select>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Fator RH: </td>
                    <td>
                        <select class="CampoEstilo" name="fdpfatorrh" id="fdpfatorrh" value="<? $dados[0]['fdpfatorrh']; ?>">
                            <option value="">Selecione...</option>
                            <option <? if ($dados[0]['fdpfatorrh'] == 'Positivo') echo "selected=selected"; ?> value="Positivo">Positivo</option>
                            <option <? if ($dados[0]['fdpfatorrh'] == 'Negativo') echo "selected=selected"; ?>  value="Negativo">Negativo</option>
                        </select>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Cor/Origem: </td>
                    <td>
                        <select class="CampoEstilo" name="fdpcor" id="fdpcor" value="<? $dados[0]['fdpcor']; ?>">
                            <option value="">Selecione...</option>
                            <option <? if ($dados[0]['fdpcor'] == 'Branca') echo "selected=selected"; ?> value="Branca">Branca</option>
                            <option <? if ($dados[0]['fdpcor'] == 'Afro') echo "selected=selected"; ?> value="Afro">Afro</option>
                            <option <? if ($dados[0]['fdpcor'] == 'Amarela') echo "selected=selected"; ?>value="Amarela">Amarela</option>
                            <option <? if ($dados[0]['fdpcor'] == 'Parda') echo "selected=selected"; ?>value="Parda">Parda</option>
                            <option <? if ($dados[0]['fdpcor'] == 'Indígena') echo "selected=selected"; ?>value="Indígena">Indígena</option>
                        </select>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Pessoa com Deficiência: </td>
                    <td>
                        <input type="radio" name="fdpdeficiente" id="fdpdeficiente[t]"  <? if ($dados[0]['fdpdeficiente'] == 't') echo 'checked = checked'; ?> value="t"> Sim  &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                        <input type="radio" name="fdpdeficiente" id="fdpdeficiente[f]"  <? if ($dados[0]['fdpdeficiente'] == 'f') echo 'checked = checked'; ?> value="f"> Não
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Tipo de Deficiência: </td>
                    <td>
                        <?PHP
                            $fdpdeficiencia = $dados[0]['fdpdeficiencia'];
                            echo campo_texto('fdpdeficiencia', 'N', $somenteLeitura, '', 50, 300, '', '', 'left', '', 0, 'id="fdpdeficiencia" onblur="MouseBlur(this);"');
                        ?>
                    </td>
                </tr>
        <?PHP
            }else{
        ?>

                <input type="hidden" name="fdpgruposanguineo" id="fdpgruposanguineo" value="NI">
                <input type="hidden" name="fdpfatorrh" id="fdpfatorrh" value="NI">
                <input type="hidden" name="fdpcor" id="fdpcor" value="NI">

        <?PHP
            }

            if( $_REQUEST['opc'] != 'CC' ){
        ?>
                <tr>
                    <td colspan="2" class="SubTituloEsquerda"> DOCUMENTOS </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Carteira de Identidade Nº: </td>
                    <td>
                        <?PHP
                            $fdpidentidade = $dados[0]['fdpidentidade'];
                            echo campo_texto('fdpidentidade', 'S', $somenteLeitura, '', 50, 20, '', '', 'left', '', 0, 'id="fdpidentidade"  onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" onblur="MouseBlur(this);"');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Órgão Expedidor Identidade: </td>
                    <td>
                        <?PHP
                            $fdpidexpeditor = $dados[0]['fdpidexpeditor'];
                            echo campo_texto('fdpidexpeditor', 'S', $somenteLeitura, '', 50, 10, '', '', 'left', '', 0, 'id="fdpidexpeditor" onblur="MouseBlur(this);"');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> UF Identidade: </td>
                    <td>
                        <?PHP
                            $fdpufidentidade = $dados[0]['fdpufidentidade'];
                            $sql = "
                                SELECT  estuf AS codigo,
                                        estdescricao AS descricao
                                FROM territorios.estado
                            ";
                            $db->monta_combo('fdpufidentidade', $sql, 'S', "Selecione...", '', '', '', '340', 'S', 'fdpufidentidade');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Data Exp. Identidade: </td>
                    <td>
                        <?PHP
                            $fdpiddataexpedicao = $dados[0]['fdpiddataexpedicao'];
                            echo campo_data2('fdpiddataexpedicao', 'S', 'S', '', 'S');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Registro Profissional (Conselho): </td>
                    <td>
                        <?PHP
                            $fdpregistroprofissional = $dados[0]['fdpregistroprofissional'];
                            echo campo_texto('fdpregistroprofissional', 'N', $somenteLeitura, '', 50, 20, '', '', 'left', '', 0, 'id="fdpregistroprofissional" onblur="MouseBlur(this);"');
                        ?>
                    </td>
                </tr>

                <tr>
                    <td class ="SubTituloDireita" align="right">Data Exp. Registro: </td>
                    <td>
                        <?PHP
                            $fdpdataregistroprofissional = $dados[0]['fdpdataregistroprofissional'];
                            echo campo_data2('fdpdataregistroprofissional', 'N', 'S', '', 'S');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right">Data. 1º Emprego: </td>
                    <td>
                        <?PHP
                            $fdpdataprimeiroemprego = $dados[0]['fdpdataprimeiroemprego'];
                            echo campo_data2('fdpdataprimeiroemprego', 'N', 'S', '', 'S');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Título de Eleitor: </td>
                    <td>
                        <?PHP
                            $fdptituloeleitor = $dados[0]['fdptituloeleitor'];
                            echo campo_texto('fdptituloeleitor', 'N', $somenteLeitura, '', 50, 20, '', '', 'left', '', 0, 'id="fdptituloeleitor" onblur="MouseBlur(this);"');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Zona:</td>
                    <td>
                        <?PHP
                            $fdpzonaeleitoral = $dados[0]['fdpzonaeleitoral'];
                            echo campo_texto('fdpzonaeleitoral', 'N', $somenteLeitura, '', 50, 20, '', '', 'left', '', 0, 'id="fdpzonaeleitoral" onblur="MouseBlur(this);"');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Seção: </td>
                    <td>
                        <?PHP
                            $fdpsecaoeleitoral = $dados[0]['fdpsecaoeleitoral'];
                            echo campo_texto('fdpsecaoeleitoral', 'N', $somenteLeitura, '', 50, 20, '', '', 'left', '', 0, 'id="fdpsecaoeleitoral" onblur="MouseBlur(this);"');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class ="SubTituloDireita" align="right"> Local: </td>
                    <td>
                        <?PHP
                            $fdplocaleleitoral = $dados[0]['fdplocaleleitoral'];
                            echo campo_texto('fdplocaleleitoral', 'N', $somenteLeitura, '', 50, 50, '', '', 'left', '', 0, 'id="fdplocaleleitoral" onblur="MouseBlur(this);"');
                        ?>
                    </td>
                </tr>
        <?PHP
            }else{
        ?>
                <input type="hidden" name="fdpidentidade" id="fdpidentidade" value="NI">
                <input type="hidden" name="fdpidexpeditor" id="fdpidexpeditor" value="NI">
                <input type="hidden" name="fdpufidentidade" id="fdpufidentidade" value="NI">
                <input type="hidden" name="fdpiddataexpedicao" id="fdpiddataexpedicao" value="<?=date('d/m/Y');?>">
                <input type="hidden" name="fdptituloeleitor" id="fdptituloeleitor" value="NI">
        <?PHP
            }

            if (!isset($_GET['pop'])) {
        ?>
                <tr>
                    <td colspan="2" class="SubTituloCentro">
                        <input type="button" name="btSalvar" id="btSalvar" onclick="validaForm();" value="Salvar" <?= $bloquearEdicao; ?>>
                        
                        <?PHP
                            if( in_array(PERFIL_AVAL_SERV_ADMINISTRADOR, $perfil) || in_array(PERFIL_FT_ADMINISTRADOR_GERAL, $perfil) || in_array(PERFIL_SUPER_USER, $perfil) ){
                        ?>
                                <input type="button" name="btVoltar" id="btVoltar" onclick="voltarPagina();" value="Voltar">
                        <?PHP
                            }
                        ?>
                    </td>
                </tr>
        <?PHP
            }
        ?>
    </table>
</form>

<script type="text/javascript">
    var fdpcpf                      = document.getElementById('fdpcpf');
    var fdpnome                     = document.getElementById('fdpnome');
    var fdpsiape                    = document.getElementById('fdpsiape');
    var eciid                       = document.getElementById('eciid');
    var fdpconjuge                  = document.getElementById('fdpconjuge');
    var fdpnomepai                  = document.getElementById('fdpnomepai');
    var fdpnomemae                  = document.getElementById('fdpnomemae');
    var fdpdatanascimento           = document.getElementById('fdpdatanascimento');
    var fdpnaturalidade             = document.getElementById('fdpnaturalidade');
    var fdpenderecoresidencial      = document.getElementById('fdpenderecoresidencial');
    var fdpbairro                   = document.getElementById('fdpbairro');
    var estuf                       = document.getElementById('estuf');
    var muncod                      = document.getElementById('muncod');
    var fdpcep                      = document.getElementById('fdpcep');
    var fdpdddtelefoneres           = document.getElementById('fdpdddtelefoneres');
    var fdptelefoneres              = document.getElementById('fdptelefoneres');
    var fdpdddcelular               = document.getElementById('fdpdddcelular');
    var fdpcelular                  = document.getElementById('fdpcelular');
    var fdpgruposanguineo           = document.getElementById('fdpgruposanguineo');
    var fdpfatorrh                  = document.getElementById('fdpfatorrh');
    var fdpcor                      = document.getElementById('fdpcor');
    var fdpdeficiencia              = document.getElementById('fdpdeficiencia');
    var fdpidentidade               = document.getElementById('fdpidentidade');
    var fdpidexpeditor              = document.getElementById('fdpidexpeditor');
    var fdpufidentidade             = document.getElementById('fdpufidentidade');
    var fdpiddataexpedicao          = document.getElementById('fdpiddataexpedicao');
    var fdpregistroprofissional     = document.getElementById('fdpregistroprofissional');
    var fdpdataregistroprofissional = document.getElementById('fdpdataregistroprofissional');
    var fdptituloeleitor            = document.getElementById('fdptituloeleitor');
    var fdpzonaeleitoral            = document.getElementById('fdpzonaeleitoral');
    var fdpsecaoeleitoral           = document.getElementById('fdpsecaoeleitoral');
    var fdplocaleleitoral           = document.getElementById('fdplocaleleitoral');
    var fdpserieeleitoral           = document.getElementById('fdpserieeleitoral');
    var fdpdataprimeiroemprego      = document.getElementById('fdpdataprimeiroemprego');
    
    var ftptipogratificacao_PE      = document.getElementById('ftptipogratificacao_PE');
    var ftptipogratificacao_CE      = document.getElementById('ftptipogratificacao_CE');
    var ftptipogratificacao_PS      = document.getElementById('ftptipogratificacao_PS');
    var ftptipogratificacao_OS      = document.getElementById('ftptipogratificacao_OS');

    var fstid = document.getElementById('fstid');
    

    var tipo_opc = document.getElementById('tipo_opc');

    function carregaDadosWS(cpf) {
        if (!cpf) {
            return false;
        }

        var comp = new dCPF();
        comp.buscarDados(cpf);
        
        var anonasc = comp.dados.dt_nascimento_rf.substr(0, 4);
        var mesnasc = comp.dados.dt_nascimento_rf.substr(4, 2);
        var dianasc = comp.dados.dt_nascimento_rf.substr(6, 7);
        var datanasc = dianasc + '/' + mesnasc + '/' + anonasc;
        
        fdpnome.value           = comp.dados.no_pessoa_rf;
        fdpcep.value            = comp.dados.nu_cep;
        fdpidentidade.value     = comp.dados.nu_rg;
        fdpidexpeditor.value    = comp.dados.ds_orgao_expedidor_rg;
        
        fdpdddcelular.value     = comp.dados.ds_contato_pessoa.substr(0,2);
        fdpcelular.value        = comp.dados.ds_contato_pessoa.substr(3,8);
        
        fdpdatanascimento.value         = datanasc;
        fdpenderecoresidencial.value    = comp.dados.ds_logradouro + ' ' + comp.dados.ds_logradouro_comp + ' ' + comp.dados.ds_numero;
        fdpbairro.value                 = comp.dados.ds_bairro;
        muncod.value                    = Number(comp.dados.co_cidade);

        filtraEstado(Number(comp.dados.co_cidade));
        
        if( tipo_opc.value != 'CC' ){
            fdpnomemae.value            = comp.dados.no_mae_rf;
            fdpcep.value                = comp.dados.nu_cep;
            fdpidentidade.value         = comp.dados.nu_rg;
            fdpidexpeditor.value        = comp.dados.ds_orgao_expedidor_rg;
            fdpiddataexpedicao.value    = comp.dados.dt_emissao_rg;
            fdptituloeleitor.value      = comp.dados.nu_titulo_eleitor_rf;
        }
    }

    function VerificaUsuarioExixte( cpf ){
        var cpf = replaceAll( replaceAll( cpf, "-", ""), ".", "");

        var req = new Ajax.Request(
            'gestaopessoa.php?modulo=principal/forca_trabalho/cad_dados_pessoais&acao=A', {
            method : 'post',
            parameters : '&VerificaUsuarioExixte='+cpf,
            onComplete: function (res){
                if( res == 'EX' ){
                    if( tipo_opc.value != 'CC' ){
                        window.location.href = '?modulo=principal/forca_trabalho/cad_dados_pessoais&acao=A';
                    }else{
                        window.location.href = '?modulo=principal/forca_trabalho/cad_dados_pessoais&acao=A&opc=CC';
                    }
                }else{
                    carregaDadosWS(cpf);
                }
            }
        });
    }

    function validaForm() {
        var muncod = document.getElementById('muncod');

        if (fdpcpf.value == '') {
            alert('O campo "CPF" não pode ser vazio');
            fdpcpf.style.backgroundColor = '#FFF68F';
            fdpcpf.focus();
            return false;
        }
        if (fstid.value == '') {
            alert('O campo "Situação no MEC" não pode ser vazio');
            fstid.style.backgroundColor = '#FFF68F';
            fstid.focus();
            return false;
        }

        if( tipo_opc.value != 'CC' ){            
            if( !ftptipogratificacao_PE.checked && !ftptipogratificacao_CE.checked && !ftptipogratificacao_PS.checked && !ftptipogratificacao_OS.checked ){
                alert('O campo "Gratificação" não pode ser vazio');
                ftptipogratificacao_PE.style.backgroundColor = '#FFF68F';
                ftptipogratificacao_PE.focus();
                return false;
            }
        }

        if (fdpnome.value == '') {
            alert('O campo "Nome" não pode ser vazio');
            fdpnome.style.backgroundColor = '#FFF68F';
            fdpnome.focus();
            return false;
        }

        if ((fdpsiape.value == '') && (fstid.value != '6')) {
            alert('O campo "SIAPE" não pode ser vazio');
            fdpsiape.style.backgroundColor = '#FFF68F';
            fdpsiape.focus();
            return false;
        }

        if (eciid.value == '') {
            alert('O campo "Estado Cívil" não pode ser vazio');
            eciid.style.backgroundColor = '#FFF68F';
            eciid.focus();
            return false;
        }

        if (fdpnomemae.value == '') {
            alert('O campo "Nome da Mãe" não pode ser vazio');
            fdpnomemae.style.backgroundColor = '#FFF68F';
            fdpnomemae.focus();
            return false;
        }

        if (document.getElementById('fdpsexo[m]').checked == 0 && document.getElementById('fdpsexo[f]').checked == 0) {
            alert('O campo "Sexo" deve ser respondido');
            document.getElementById('fdpsexo[m]').style.backgroundColor = '#FFF68F';
            document.getElementById('fdpsexo[m]').focus();
            return false;
        }

        if (fdpdatanascimento.value == '') {
            alert('O campo "Data de nascimento" não pode ser vazio');
            fdpdatanascimento.style.backgroundColor = '#FFF68F';
            fdpdatanascimento.focus();
            return false;
        }
        if (fdpnaturalidade.value == '') {
            alert('O campo "Naturalidade" não pode ser vazio');
            fdpnaturalidade.style.backgroundColor = '#FFF68F';
            fdpnaturalidade.focus();
            return false;
        }
        if (fdpenderecoresidencial.value == '') {
            alert('O campo "Endereço residencial" não pode ser vazio');
            fdpenderecoresidencial.style.backgroundColor = '#FFF68F';
            fdpenderecoresidencial.focus();
            return false;
        }
        if (fdpbairro.value == '') {
            alert('O campo "Bairro" não pode ser vazio');
            fdpbairro.style.backgroundColor = '#FFF68F';
            fdpbairro.focus();
            return false;
        }
        if (estuf.value == '') {
            alert('O campo "UF" não pode ser vazio');
            estuf.style.backgroundColor = '#FFF68F';
            estuf.focus();
            return false;
        }
        if (muncod.value == '') {
            alert('O campo "Município" não pode ser vazio');
            muncod.style.backgroundColor = '#FFF68F';
            muncod.focus();
            return false;
        }
        if (fdpcep.value == '') {
            alert('O campo "CEP" não pode ser vazio');
            fdpcep.style.backgroundColor = '#FFF68F';
            fdpcep.focus();
            return false;
        }

        if( tipo_opc.value != 'CC' ){
            if (fdpdatanascimento.value.length < 10) {
                alert('Insira uma data de nascimento no formato \'dd/mm/aaaa\'.');
                fdpdatanascimento.style.backgroundColor = '#FFF68F';
                fdpdatanascimento.focus();
                return false;
            }
            if (!validaData(fdpdatanascimento)) {
                alert('O campo "Data de nascimento" inválido.');
                fdpdatanascimento.style.backgroundColor = '#FFF68F';
                fdpdatanascimento.focus();
                return false;
            }
            if (document.getElementById('fdpdeficiente[t]').checked == 0 && document.getElementById('fdpdeficiente[f]').checked == 0){
                alert('O campo "Possui deficiência" deve ser respondido');
                document.getElementById('fdpdeficiente[t]').style.backgroundColor = '#FFF68F';
                document.getElementById('fdpdeficiente[t]').focus();
                return false;
            }
            if (fdpidentidade.value == '') {
                alert('O campo "Identidade" não pode ser vazio');
                fdpidentidade.style.backgroundColor = '#FFF68F';
                fdpidentidade.focus();
                return false;
            }
            if (fdpidexpeditor.value == '') {
                alert('O campo "Orgão expeditor" não pode ser vazio');
                fdpidexpeditor.style.backgroundColor = '#FFF68F';
                fdpidexpeditor.focus();
                return false;
            }
            if (fdpufidentidade.value == '') {
                alert('O campo "UF orgão expeditor" não pode ser vazio');
                fdpufidentidade.style.backgroundColor = '#FFF68F';
                fdpufidentidade.focus();
                return false;
            }
            if (fdpiddataexpedicao.value == '') {
                alert('O campo "Data de expedicao UF" não pode ser vazio');
                fdpiddataexpedicao.style.backgroundColor = '#FFF68F';
                fdpiddataexpedicao.focus();
                return false;
            }
            if ((fdpiddataexpedicao.value.length > 0) && (fdpiddataexpedicao.value.length < 10)) {
                alert('Insira uma data de expedicao no formato \'dd/mm/aaaa\'.');
                fdpiddataexpedicao.style.backgroundColor = '#FFF68F';
                fdpiddataexpedicao.focus();
                return false;
            }
            if (!validaData(fdpiddataexpedicao)) {
                alert('O campo "Data de expedicao UF inválido');
                fdpiddataexpedicao.style.backgroundColor = '#FFF68F';
                fdpiddataexpedicao.focus();
                return false;
            }
            if ((fdpdataregistroprofissional.value.length > 0) && (fdpdataregistroprofissional.value.length < 10)) {
                alert('Insira uma data de registro profissional no formato \'dd/mm/aaaa\'.');
                fdpdataregistroprofissional.style.backgroundColor = '#FFF68F';
                fdpdataregistroprofissional.focus();
                return false;
            }
            if (fdpdataregistroprofissional.value) {
                if (!validaData(fdpdataregistroprofissional)) {
                    alert('O campo "Data de registro profissional inválido');
                    fdpdataregistroprofissional.style.backgroundColor = '#FFF68F';
                    fdpdataregistroprofissional.focus();
                    return false;
                }
            }
            if ((fdpdataprimeiroemprego.value.length > 0) && (fdpdataprimeiroemprego.value.length < 10)) {
                alert('Insira uma data de 1º emprego no formato \'dd/mm/aaaa\'.');
                fdpdataprimeiroemprego.style.backgroundColor = '#FFF68F';
                fdpdataprimeiroemprego.focus();
                return false;
            }
            if (fdpdataprimeiroemprego.value) {
                if (!validaData(fdpdataprimeiroemprego)) {
                    alert('O campo "Data de 1º emprego inválido');
                    fdpdataprimeiroemprego.style.backgroundColor = '#FFF68F';
                    fdpdataprimeiroemprego.focus();
                    return false;
                }
            }

            if (fdplocaleleitoral.value.length > 0) {
                if (fdplocaleleitoral.value.length > 50) {
                    alert("Quantidade de cadacteres excedida para o campo \"Local Eleitoral\"");
                    fdplocaleleitoral.style.backgroundColor = '#FFF68F';
                    fdplocaleleitoral.focus();
                    return false;
                }
            }
        }

        document.getElementById('tipo_acao').value = 'GRAVAR';
        document.formulario.submit();
    }

    function filtraMunicipio(estuf){
        if (!estuf) {
            return false;
        }
        var td_municipio = document.getElementById('td_municipio');
        var td_estado = document.getElementById('td_estado');

        var req = new Ajax.Request('gestaopessoa.php?modulo=principal/forca_trabalho/cad_dados_pessoais&acao=A', {
            method: 'post',
            parameters: '&ajaxestuf=' + estuf,
            onComplete: function(res){
                td_municipio.innerHTML = res.responseText;
            }
        });
    }

    function RmvFoto(){
        if (confirm('Deseja realmente remover a foto?')) {
            var req = new Ajax.Request('gestaopessoa.php?modulo=principal/forca_trabalho/cad_dados_pessoais&acao=A', {
                method: 'post',
                parameters: '&ajaxremovefoto=1',
                onComplete: function(res){
                    if (res.responseText == 'OK'){
                        window.location.reload();
                    }
                }
            });
        }
    }

    function filtraEstado(muncod) {
        if (!muncod) {
            return false;
        }
        var td_municipio = document.getElementById('td_municipio');
        var td_estado = document.getElementById('td_estado');
        var estuf = document.getElementById('estuf');

        var req = new Ajax.Request('gestaopessoa.php?modulo=principal/forca_trabalho/cad_dados_pessoais&acao=A', {
            method: 'post',
            parameters: '&ajaxmuncod=' + muncod,
            onComplete: function(res){
                estuf.value = res.responseText;
            }
        });
    }

    var http = createRequestObject();
    var uploader = "";
    var uploadDir = "";
    var dirname = "";
    var filename = "";
    var timeInterval = "";
    var idname = "";
    var uploaderId = "";

    function createRequestObject() {
        var obj;
        var browser = navigator.appName;
        if (browser == "Microsoft Internet Explorer") {
            document.getElementById('div_files').style.left = "-446px";
            document.getElementById('div_files').style.top = "60px";
            document.getElementById('imagem').style.left = "0px";
            document.getElementById('imagem').style.top = "0px";
            return new ActiveXObject("Microsoft.XMLHTTP");
        }
        else {
            return new XMLHttpRequest();
        }
    }

    function traceUpload(){
        http.onreadystatechange = handleResponse;
        http.open("GET", 'gestaopessoa.php?modulo=principal/imageupload&acao=I&uploadDir=' + uploadDir + '&dirname=' + dirname + '&filename=' + filename + '&uploader=' + uploader);
        http.send(null);
    }

    function handleResponse(){
        if (http.readyState == 4) {
            var response = http.responseText;

            if (response != '0') {
                clearInterval(timeInterval);
            }

            if (response.indexOf("#") != -1) {
                arResponse = response.split("#");
                document.getElementById('imagem').innerHTML = arResponse[0];
                document.getElementById('div_files').innerHTML = arResponse[1];
                document.getElementById('removefoto').innerHTML = "<a href='javascript:RmvFoto()'><b><font color=red>(x) Remover</font></b></a>";
            }
            else {
                document.getElementById('imagem').innerHTML = 'Erro Upload, aperte F5.';
                document.getElementById('div_files').innerHTML = '';
                document.getElementById('removefoto').innerHTML = '';
            }
        }
        else {
            document.getElementById('imagem').innerHTML = "<img src='/imagens/wait.gif' align=\"center\" border\"0\">";
            document.getElementById(uploaderId).innerHTML = "Uploading File. Please wait...";
        }
    }

    function uploadFile(obj, dname){
        uploadDir = obj.value;
        idname = obj.name;
        dirname = dname;

        filename = uploadDir.substr(uploadDir.lastIndexOf('\\') + 1);

        uploaderId = 'uploader' + obj.name;
        uploader = obj.name;
        var df = document.getElementById('formName' + obj.name);
        document.getElementById('formName' + obj.name).submit();

        //verifica os tipos válidos;
        var arFileName = filename.split(".");
        if ((arFileName[1] != 'jpeg') && (arFileName[1] != 'jpg') && (arFileName[1] != 'gif') && (arFileName[1] != 'png') && (arFileName[1] != 'bmp')) {
            alert('O arquivo informado não foi reconhecido como um arquivo de imagem. Favor utilizar uma das extensões:\n\njpg, gif, png ou bmp');
            return false;
        }
        //chama a função com intervalo de 1500 ms
        timeInterval = setInterval("traceUpload()", 1500);
    }

    function voltarPagina(){
        window.location.href ='gestaopessoa.php?modulo=principal/forca_trabalho/lista_grid_chefia_servidores_cedido&acao=A';
    }
</script>