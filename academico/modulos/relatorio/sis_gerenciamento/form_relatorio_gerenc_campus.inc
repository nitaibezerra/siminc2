<?PHP
    ini_set("memory_limit","256M");
    include 'resul_relatorio_gerenc_campus.inc';

    if ($_REQUEST['painel']){
        extract ($_REQUEST);
    }
    
    /* Componente Historico de consulta.
     * Salva os POST da Consulta efetuado
    * Código implementado em 17/08/2012.
    * Luciano F. Ribeiro.
    */
    if($_REQUEST['requisicao'] == 'salvarConsulta'){
        $existe_rel = 0;

        $sql = "select prtid from public.parametros_tela where prtdsc = '".$_POST['titulo_consulta']."' and usucpf ='".$_SESSION['usucpf']."'and mnuid =".$_SESSION['mnuid'].";";

        $existe_rel = $db->pegaUm( $sql );

        if ($existe_rel > 0){
            $sql = sprintf(
                "UPDATE public.parametros_tela SET prtdsc = '%s', prtobj = '%s', prtpublico = 'FALSE', usucpf = '%s', mnuid = %d, prtobservacao = '%s' WHERE prtid = %d", $_POST['titulo_consulta'],
                addslashes( addslashes( serialize( $_REQUEST ) ) ),
                $_SESSION['usucpf'],
                $_SESSION['mnuid'],
                $_POST['obs_consulta'],
                $existe_rel
            );
            $db->executar( $sql );
            $db->commit();
        }else{
            $sql = sprintf(
                "INSERT INTO public.parametros_tela ( prtdsc, prtobj, prtpublico, usucpf, mnuid, prtobservacao) VALUES ( '%s', '%s', %s, '%s', %d, '%s' )", $_POST['titulo_consulta'],
                addslashes( addslashes( serialize( $_REQUEST ) ) ),
                'FALSE',
                $_SESSION['usucpf'],
                $_SESSION['mnuid'],
                $_POST['obs_consulta']
            );
            $db->executar( $sql );
            $db->commit();
        }
        $db->sucesso('relatorio/sis_gerenciamento/form_relatorio_gerenc_campus');
        die;
    }

    /* Componente Historico de consulta.
     * Transforma consulta em pública.
     * Código implementado em 17/08/2012.
     * Luciano F. Ribeiro.
     */
    if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
        $sql = sprintf(
            "UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d", $_REQUEST['prtid']
        );
        $db->executar( $sql );
        $db->commit();
        $db->sucesso('relatorio/sis_gerenciamento/form_relatorio_gerenc_campus');
        die;
    }

    /* Componente Historico de consulta.
     * Exclui consulta.
     * Código implementado em 17/08/2012.
     * Luciano F. Ribeiro.
     */
    if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
        $sql = sprintf(
            "DELETE from public.parametros_tela WHERE prtid = %d",
            $_REQUEST['prtid']
        );
        $db->executar( $sql );
        $db->commit();
        $db->sucesso('relatorio/sis_gerenciamento/form_relatorio_gerenc_campus');
        die;
    }

    /* Componente Historico de consulta.
     * Carrega a consulta salva no historico, consulta dados em banco.
     * Código implementado em 17/08/2012.
     * Luciano F. Ribeiro.
     */
    if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){
        $sql   = sprintf("select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid']);
        $itens = $db->pegaUm( $sql );
        $dados = unserialize( stripslashes( stripslashes($itens) ) );

        extract($dados);
        $_REQUEST = $dados;
        $_POST = $dados;
    }

    ini_set("memory_limit","500M");
    set_time_limit(0);

    $origem[TPENSPROF]  = array();
    
    $destino[TPENSPROF] = array(
        'estuf' => array(
            'codigo' => 'estuf',
            'descricao' => 'UF'
        ),
        'mundescricao' => array(
            'codigo'    => 'mundescricao',
            'descricao' => 'Município'
        ),
        'muncod' => array(
            'codigo'    => 'muncod',
            'descricao' => 'Código do município'
        ),
        'entid' => array(
            'codigo'    => 'entid',
            'descricao' => 'Código do campus'
        ),
        'entunicod' => array(
            'codigo'    => 'entunicod',
            'descricao' => 'Código da unidade (UO)'
        ),
        'unidadeorc' => array(
            'codigo'    => 'unidadeorc',
            'descricao' => 'Instituto'
        ),
        'entnome' => array(
            'codigo'    => 'entnome',
            'descricao' => 'Campus'
        ),
        'entsig' => array(
            'codigo'    => 'entsig',
            'descricao' => 'Sigla da unidade'
        ),
        'exidsc' => array(
            'codigo'    => 'exidsc',
            'descricao' => 'Existência'
        ),
        'cmpsituacao' => array(
            'codigo'    => 'cmpsituacao',
            'descricao' => 'Situação'
        ),
        'cmpinstalacao' => array(
            'codigo'    => 'cmpinstalacao',
            'descricao' => 'Instalações'
        ),
        'obrascampus' => array(
            'codigo'    => 'obrascampus',
            'descricao' => 'Obras no campus'
        ),
        'cmpsituacaoobra' => array(
            'codigo'    => 'cmpsituacaoobra',
            'descricao' => 'Situação da obra no campus/uned'
        ),
        'matriculas' => array(
            'codigo'    => 'matriculas',
            'descricao' => 'MATRÍCULAS'
        ),
        'cptdsc' => array(
            'codigo'    => 'cptdsc',
            'descricao' => 'Tipo'
        ),
        'datacriacao' => array(
            'codigo'    => 'datacriacao',
            'descricao' => 'Data de criação'
        ),
        'cmpdataimplantacao' => array(
            'codigo'    => 'cmpdataimplantacao',
            'descricao' => 'Data do início das atividades'
        ),
        'cmpdatainauguracao' => array(
            'codigo'    => 'cmpdatainauguracao',
            'descricao' => 'Data de Inauguração'
        ),
        'insprevisto' => array(
            'codigo'    => 'insprevisto',
            'descricao' => 'Investimento atual (Previsto)'
        ),
        'insrealizado' => array(
            'codigo'    => 'insrealizado',
            'descricao' => 'Investimento total (Realizado)'
        ),
        'insrealizado2005_2010' => array(
            'codigo'    => 'insrealizado2005_2010',
            'descricao' => 'Investimentos realizados 2005/2010'
        ),
        'insprevisto2011_2014' => array(
            'codigo'    => 'insprevisto2011_2014',
            'descricao' => 'Investimentos previstos 2011/2014'
        ),
        'insrealizadomaior2011' => array(
            'codigo'    => 'insrealizadomaior2011',
            'descricao' => 'Investimentos realizados a partir de 2011'
        ),
        'dirigentecampus' => array(
            'codigo'    => 'dirigentecampus',
            'descricao' => 'Dirigente do campus'
        ),
        'interlocutorcampus' => array(
            'codigo'    => 'interlocutorcampus',
            'descricao' => 'Interlocutor do campus'
        ),
        'enderecocampus' => array(
            'codigo'    => 'enderecocampus',
            'descricao' => 'Endereço do Campus'
        ),
        'enderecounidadeorc' => array(
            'codigo'    => 'enderecounidadeorc',
            'descricao' => 'Endereço do Instituto'
        ),
        'emailcampus' => array(
            'codigo'    => 'emailcampus',
            'descricao' => 'E-mail do campus'
        ),
        'telefonecampus' => array(
            'codigo'    => 'telefonecampus',
            'descricao' => 'Telefone do campus'
        ),
        'latitude' => array(
            'codigo'    => 'latitude',
            'descricao' => 'Latitude'
        ),
        'longitude' => array(
            'codigo'    => 'longitude',
            'descricao' => 'Longitude'
        )
    );

    $origem[TPENSSUP] = array();
    $destino[TPENSSUP] = array(
        'estuf' => array(
            'codigo'    => 'estuf',
            'descricao' => 'UF'
        ),
        'mundescricao' => array(
            'codigo'    => 'mundescricao',
            'descricao' => 'Município'
        ),
        'muncod' => array(
            'codigo'    => 'muncod',
            'descricao' => 'Código do município'
        ),
        'entid' => array(
            'codigo'    => 'entid',
            'descricao' => 'Código do campus'
        ),
        'entunicod' => array(
            'codigo'    => 'entunicod',
            'descricao' => 'Código da unidade (UO)'
        ),
        'unidadeorc' => array(
            'codigo'    => 'unidadeorc',
            'descricao' => 'Universidade'
        ),
        'entnome' => array(
            'codigo'    => 'entnome',
            'descricao' => 'Campus'
        ),
        'entsig' => array(
            'codigo'    => 'entsig',
            'descricao' => 'Sigla da unidade'
        ),
        'exidsc' => array(
            'codigo'    => 'exidsc',
            'descricao' => 'Existência'
        ),
        'cmpsituacao' => array(
            'codigo'    => 'cmpsituacao',
            'descricao' => 'Situação'
        ),
        'cmpinstalacao' => array(
            'codigo'    => 'cmpinstalacao',
            'descricao' => 'Instalações'
        ),
        'obrascampus' => array(
            'codigo'    => 'obrascampus',
            'descricao' => 'Obras no campus'
        ),
        'docenteprevisto' => array(
            'codigo'    => 'docenteprevisto',
            'descricao' => 'DOCENTE (PREVISTO)'
        ),
        'tecnicoprevisto' => array(
            'codigo'    => 'tecnicoprevisto',
            'descricao' => 'TÉCNICO (PREVISTO)'
        ),
        'matprevisto' => array(
            'codigo'    => 'matprevisto',
            'descricao' => 'MAT (PREVISTO)'
        ),
        'vagasprevisto' => array(
            'codigo'    => 'vagasprevisto',
            'descricao' => 'VAGAS (PREVISTO)'
        ),
        'cursoprevisto' => array(
            'codigo'    => 'cursoprevisto',
            'descricao' => 'CURSO (PREVISTO)'
        ),
        'investimentoprevisto' => array(
            'codigo'    => 'investimentoprevisto',
            'descricao' => 'INVESTIMENTO (PREVISTO)'
        ),
        'bolsasmestradoprevisto' => array(
            'codigo'    => 'bolsasmestradoprevisto',
            'descricao' => 'BOLSAS DE MESTRADO (PREVISTO)'
        ),
        'bolsasdoutoradoprevisto' => array(
            'codigo'    => 'bolsasdoutoradoprevisto',
            'descricao' => 'BOLSAS DE DOUTORADO (PREVISTO)'
        ),
        'bolsasposdoutoradoprevisto' => array(
            'codigo'    => 'bolsasposdoutoradoprevisto',
            'descricao' => 'BOLSAS DE PÓS-DOUTORADO (PREVISTO)'
        ),
        'docenterealizado' => array(
            'codigo'    => 'docenterealizado',
            'descricao' => 'DOCENTE (REALIZADO)'
        ),
        'tecnicorealizado' => array(
            'codigo'    => 'tecnicorealizado',
            'descricao' => 'TÉCNICO (REALIZADO)'
        ),
//        'matrealizado' => array(
//            'codigo'    => 'matrealizado',
//            'descricao' => 'MAT (REALIZADO)'
//        ),
        'vagasrealizado' => array(
            'codigo'    => 'vagasrealizado',
            'descricao' => 'VAGAS (REALIZADO)'
        ),
        'cursorealizado' => array(
            'codigo'    => 'cursorealizado',
            'descricao' => 'CURSO (REALIZADO)'
        ),
        'investimentorealizado' => array(
            'codigo'    => 'investimentorealizado',
            'descricao' => 'INVESTIMENTO (REALIZADO)'
        ),
        'bolsasmestradorealizado' => array(
            'codigo'    => 'bolsasmestradorealizado',
            'descricao' => 'BOLSAS DE MESTRADO (REALIZADO)'
        ),
        'bolsasdoutoradorealizado' => array(
            'codigo'    => 'bolsasdoutoradorealizado',
            'descricao' => 'BOLSAS DE DOUTORADO (REALIZADO)'
        ),
        'bolsasposdoutoradorealizado' => array(
            'codigo'    => 'bolsasposdoutoradorealizado',
            'descricao' => 'BOLSAS DE PÓS-DOUTORADO (REALIZADO)'
        ),
        'matriculasgradpresen' => array(
            'codigo'    => 'matriculasgradpresen',
            'descricao' => 'MATRÍCULAS (GRADUAÇÃO PRESENCIAL)'
        ),
        'cptdsc' => array(
            'codigo'    => 'cptdsc',
            'descricao' => 'Tipo'
        ),
        'datacriacao' => array(
            'codigo'    => 'datacriacao',
            'descricao' => 'Data de criação'
        ),
        'cmpdataimplantacao' => array(
            'codigo'    => 'cmpdataimplantacao',
            'descricao' => 'Data do início das atividades'
        ),
        'cmpdatainauguracao' => array(
            'codigo'    => 'cmpdatainauguracao',
            'descricao' => 'Data de Inauguração'
        ),
        'insvtot' => array(
            'codigo'    => 'insvtot',
            'descricao' => 'Investimento total'
        ),
        'dirigentecampus' => array(
            'codigo'    => 'dirigentecampus',
            'descricao' => 'Dirigente do campus'
        ),
        'interlocutorcampus' => array(
            'codigo'    => 'interlocutorcampus',
            'descricao' => 'Interlocutor do campus'
        ),
        'enderecocampus' => array(
            'codigo'    => 'enderecocampus',
            'descricao' => 'Endereço do Campus'
        ),
        'enderecounidadeorc' => array(
            'codigo'    => 'enderecounidadeorc',
            'descricao' => 'Endereço da Universidade'
        ),
        'emailcampus' => array(
            'codigo'    => 'emailcampus',
            'descricao' => 'E-mail do campus'
        ),
        'telefonecampus' => array(
            'codigo'    => 'telefonecampus',
            'descricao' => 'Telefone do campus'
        ),
        'latitude' => array(
            'codigo'    => 'latitude',
            'descricao' => 'Latitude'
        ),
        'longitude' => array(
            'codigo'    => 'longitude',
            'descricao' => 'Longitude'
        ),
        'insrealizado2005_2010' => array(
            'codigo'    => 'insrealizado2005_2010',
            'descricao' => 'Investimentos realizados 2005/2010'
        ),
        'insprevisto2011_2014' => array(
            'codigo'    => 'insprevisto2011_2014',
            'descricao' => 'Investimentos previstos 2011/2014'
        ),
        'insrealizadomaior2011' => array(
            'codigo'    => 'insrealizadomaior2011',
            'descricao' => 'Investimentos realizados a partir de 2011'
        )
    );

    if(	($_POST['exibir'] == 'relatorio_html' || $_POST['exibir'] == 'relatorio_xls' || $_POST['exibir'] == 'relatorio_csv') ){
	$filtroSQL[] = "tuo.orgid IN('".$_POST['orgid']."')";
	$filtroSQL[] = "fen.funid IN (" . ($_POST['orgid'] == 2 ? 17 : 18) . ")";

	unset($vals);
	if($_POST['estuf'][0]) {
            foreach($_POST['estuf'] as $uf) {
                $vals[] = $uf;
            }
            $filtroSQL[] = "edc.estuf IN('".implode("','",$vals)."')";
	}
        
	unset($vals);        
	if($_POST['muncod'][0]) {
            foreach($_POST['muncod'] as $municipio) {
                $vals[] = $municipio;
            }
            $filtroSQL[] = "edc.muncod IN('".implode("','",$vals)."')";
	}
        
	unset($vals);
	if($_POST['unidades'][0]) {
            foreach($_POST['unidades'] as $uni) {
                $vals[] = $uni;
            }
            $filtroSQL[] = "uor.entid IN('".implode("','",$vals)."')";
	}
        
	if($_POST['exiid']) {
            $filtroSQL[] = "cam.exiid = '".$_POST['exiid']."'";
	}
        
	if($_POST['cmpsituacao']) {
            $filtroSQL[] = "cam.cmpsituacao = '".$_POST['cmpsituacao']."'";
	}
        
	if($_POST['cmpinstalacao']) {
            $filtroSQL[] = "cam.cmpinstalacao = '".$_POST['cmpinstalacao']."'";
	}
        
	if($_POST['cmpsituacaoobra']) {
            $filtroSQL[] = "cam.cmpsituacaoobra = '".$_POST['cmpsituacaoobra']."'";
	}
        
	/* Gerar cabeçalho XLS */
	$nomeDoArquivoXls = "relatorio_nota_tecnica_".date('d-m-Y_H_i');
	switch($_REQUEST['orgid']){
            case TPENSSUP:
                mostrar_superior($filtroSQL);
            break;
            case TPENSPROF:
                mostrar_profissional($filtroSQL);
            break;
	}
    }

    include APPRAIZ . 'includes/cabecalho.inc';
    include APPRAIZ . 'includes/Agrupador.php';
    require_once APPRAIZ . "adodb/adodb.inc.php";

    echo "<br/>";
    $titulo_modulo = "Sistema de Informações Gerenciais";
    monta_titulo( $titulo_modulo,'Relatório Nota Técnica');
?>

    <style type="text/css">
        #div_selecao {
            visibility: hidden;
            border-style: solid;
            border-width: 1px;
            borborder-color: black;
            background-color: #DCDCDC;
            width: 23%;
            height: 13%;
            position: absolute;
            top: 48%;
            left: 35%;
        }
    </style>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

    <script>
        function showColunas(v) {
            document.getElementById('coluna_1').style.display = 'none';
            document.getElementById('coluna_2').style.display = 'none';
            document.getElementById('coluna_'+v).style.display = '';
        }

        function abrirPopup(){
            window.open('', 'abrirPopup', 'toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=800,height=600');
        }

        function abrepopupUnidade(){
            for(i=0;i<document.formulario.orgid.length;i++) {
                if(document.formulario.orgid[i].checked) {
                        var itemselecionado = document.formulario.orgid[i].value;
                }
            }
            janela = window.open('/sig/combo_unidades.php?tipoensino='+itemselecionado,'Unidades','width=400,height=400,toolbar=yes,status=yes,scrollbars=1,top=200,left=480');
            janela.focus();
        }

        function exibirRelatorio(){
            document.getElementById('carregar').value = "";

            var formulario = document.formulario
            var radioselecionado = false;

            for(i=0;i<document.formulario.orgid.length;i++) {
                if(document.formulario.orgid[i].checked) {
                    radioselecionado = true;
                }
            }
            if(!radioselecionado) {
                alert('"Tipo de ensino" é obrigatório.');
                return false;
            }

            selectAllOptions(document.formulario.agrupador_1);
            selectAllOptions(document.formulario.agrupador_2);

            selectAllOptions(document.formulario.estuf);
            selectAllOptions(document.formulario.unidades);
            selectAllOptions(document.formulario.muncod);

            formulario.submit();
        }

        /*
         * Altera a visibilidade do Bloco - visualizar consulta 'ABRE' o bloco.
         * @param string indica o campo a ser mostrado/escondido
         * @return void
         */
        function onOffBloco(bloco){
            var div_on 	= document.getElementById( bloco + '_div_filtros_on' );
            var div_off = document.getElementById( bloco + '_div_filtros_off' );
            var img 	= document.getElementById( bloco + '_img' );
            var input 	= document.getElementById( bloco + '_flag' );

            if(div_on.style.display == 'none'){
                div_on.style.display = 'block';
                div_off.style.display = 'none';
                input.value = '0';
                img.src = '/imagens/menos.gif';
            }else{
                div_on.style.display = 'none';
                div_off.style.display = 'block';
                input.value = '1';
                img.src = '/imagens/mais.gif';
            }
        }

        function tornar_publico( prtid ){
            document.getElementById('publico').value = '1';
            document.getElementById('prtid').value = prtid;
            document.getElementById('formulario').action = 'academico.php?modulo=relatorio/sis_gerenciamento/form_relatorio_gerenc_campus&acao=A';
            document.getElementById('formulario').target = '_self';
            document.getElementById('formulario').submit();
       }

        function excluir_relatorio( prtid ){
            if(confirm( 'Tem certeja que deseja excluir o registro?' )){
                document.getElementById('excluir').value = '1';
                document.getElementById('prtid').value = prtid;
                document.getElementById('formulario').action = 'academico.php?modulo=relatorio/sis_gerenciamento/form_relatorio_gerenc_campus&acao=A';
                document.getElementById('formulario').target = '_self';
                document.getElementById('formulario').submit();
            }
        }

        function carregar_relatorio( prtid ){
            document.getElementById('carregar').value = '1';
            document.getElementById('prtid').value = prtid;
            document.getElementById('formulario').action = 'academico.php?modulo=relatorio/sis_gerenciamento/form_relatorio_gerenc_campus&acao=A';
            document.getElementById('formulario').target = '_blank';
            document.getElementById('formulario').submit();
        }

        function salvarConsultaEfetuada(){
            var formulario = document.formulario;

            if ( document.getElementById('titulo_consulta').value == '' ) {
                alert( 'É necessário informar a descrição do relatório!' );
                document.getElementById('titulo_consulta').focus();
                return;
            }

            jQuery("#div_selecao").css("visibility", "visible");

            var nomesExistentes = new Array();
            
            <?PHP
                $sqlNomesConsulta = "
                    SELECT prtdsc FROM public.parametros_tela Where usucpf ='{$_SESSION['usucpf']}' AND mnuid = {$_SESSION['mnuid']}
                ";
                $nomesExistentes = $db->carregar($sqlNomesConsulta);
                if ($nomesExistentes) {
                    foreach ($nomesExistentes as $linhaNome) {
                        print "nomesExistentes[nomesExistentes.length] = '" . str_replace("'", "\'", $linhaNome['prtdsc']) . "';";
                    }
                }
            ?>
                    
            var confirma = true;
            var i, j = nomesExistentes.length;
            for(i = 0; i < j; i++){
                if(nomesExistentes[i] == document.getElementById('titulo_consulta').value){
                    confirma = confirm( 'Deseja alterar a consulta já existente?');
                    break;
                }
            }
            if(!confirma){
                return;
            }

            selectAllOptions(document.getElementById('agrupador_1'));
            selectAllOptions(document.getElementById('agrupador_2'));

            selectAllOptions(document.formulario.estuf);
            selectAllOptions(document.formulario.unidades);
            selectAllOptions(document.formulario.muncod);
        }

        function escolhaRelatorio(){
            jQuery("#div_selecao").css("visibility", "hidden");
            document.getElementById('formulario').action = 'academico.php?modulo=relatorio/sis_gerenciamento/form_relatorio_gerenc_campus&acao=A&requisicao=salvarConsulta';
            document.getElementById('formulario').target = '_self';
            document.getElementById('formulario').submit();
        }
    </script>

    <form method="post" name="formulario" id="formulario" target="abrirPopup">
	<input type="hidden" name="publico" id="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" id="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" id="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" id="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	<input type="hidden" id="exibir" name="exibir" value="">

	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
            <!-- HISTORICO DE CONSULTAS - MINHAS CONSULTAS -->
            <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
                <tr>
                    <td onclick="javascript:onOffBloco('minhasconsultas');" >
                        <img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
                        Minhas Consultas
                        <input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />
                    </td>
                </tr>
            </table>

            <div name="minhasconsultas_div_filtros_off" id="minhasconsultas_div_filtros_off"></div>
            <div id="minhasconsultas_div_filtros_on" style="display:none;">
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
                    <tr>
                        <td width="195" class="SubTituloDireita" valign="top">Consultas</td>
                        <?php
                            $sql = sprintf("
                                    SELECT CASE WHEN prtpublico = false
                                                THEN '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
                                                          <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                          <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                                ELSE '<img border=\"0\" src=\"../imagens/grupo_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
                                                          <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
                                                          <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                            END as acao,

                                            '<b>' || prtdsc || '</b>' as descricao,
                                            '<b>' || prtobservacao || '</b>' as observacao

                                    FROM public.parametros_tela
                                    WHERE mnuid = %d AND usucpf = '%s'", $_SESSION['mnuid'], $_SESSION['usucpf']
                            );
                            $cabecalho = array('Ação', 'Nome', 'Observações');
                        ?>
                        <td>
                            <?PHP $db->monta_lista_simples($sql, $cabecalho, 50, 50, 'N', '95%', null); ?>
                        </td>
                    </tr>
                </table>
            </div>
            <!-- FIM - HISTORICO DE CONSULTAS - MINHAS CONSULTAS -->

            <!-- CONSULTAS PUBLICAS - CONSULTAS QUE USUÁRIOS DISPONIBILIZÃO -->
            <table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
                <tr>
                    <td onclick="javascript:onOffBloco('outros');">
                        <img border="0" src="/imagens/mais.gif" id="outros_img"/>&nbsp;
                        Relatórios Gerenciais
                        <input type="hidden" id="outros_flag" name="outros_flag" value="0" />
                    </td>
                </tr>
            </table>

            <div id="outros_div_filtros_off"></div>
            <div id="outros_div_filtros_on" style="display:none;">
                <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
                    <tr>
                        <td width="195" class="SubTituloDireita" valign="top">Relatórios:</td>
                        <td>
                            <?PHP
                                $sql = sprintf("
                                            SELECT	CASE WHEN prtpublico = true and usucpf = '%s'
                                                        THEN '<img border=\"0\" src=\"../imagens/usuario.gif\" title=\" Despublicar \" onclick=\"tornar_publico(' || prtid || ');\">&nbsp;&nbsp;
                                                                  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
                                                                  <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">'
                                                        ELSE '<img border=\"0\" src=\"../imagens/usuario_bloqueado.gif\" title=\" Sem ação \">&nbsp;&nbsp;
                                                                  <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ');\">&nbsp;&nbsp;
                                                                  <img border=\"0\" src=\"../imagens/excluir_01.gif\" title=\" Sem ação \">'
                                                    END as acao,
                                                '<b>' || prtdsc || '</b>' as descricao,
                                                '<b>' || prtobservacao || '</b>' as observacao
                                        FROM public.parametros_tela
                                        WHERE mnuid = %d and prtpublico = TRUE ", $_SESSION['usucpf'], $_SESSION['mnuid'], $_SESSION['usucpf']
                                );
                                $cabecalho = array('Ação', 'Nome', 'Observações');
                                $db->monta_lista_simples($sql, $cabecalho, 50, 50, null, null, null);
                            ?>
                        </td>
                    </tr>
                </table>
            </div>
            <!-- FIM DA CONSULTAS PUBLICAS - CONSULTAS QUE USUÁRIOS DISPONIBILIZÃO -->

            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                <tr>
                    <td class="SubTituloDireita">Definir título da consulta</td>
                    <td>
                        <?= campo_texto('titulo_consulta', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo_consulta"'); ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Definir observações para consulta</td>
                    <td>
                        <?= campo_textarea('obs_consulta', 'N', 'S', '', 69, 3, '', '', '', '', '', 'id="obs_consulta"'); ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top">Tipo de ensino</td>
                    <td>
                        <?PHP
                            $sqlTipoEnsino = "SELECT * FROM academico.orgao WHERE orgid IN (1,2) ORDER BY orgdesc";
                            $tipoen = $db->carregar($sqlTipoEnsino);
                            if ($tipoen[0]) {
                                $prim = true;
                                foreach ($tipoen as $tpe) {
                                    echo "<input type='radio' name='orgid' value='" . $tpe['orgid'] . "' " . (($prim) ? "checked" : "") . " onclick=\"if(this.checked){showColunas('" . $tpe['orgid'] . "');}\"> " . $tpe['orgdesc'] . "&nbsp;&nbsp;";
                                    $prim = false;
                                }
                            }
                        ?>
                    </td>
                </tr>
                <tr id="coluna_1" style="display:none;">
                    <td class="SubTituloDireita" valign="top" width="20%">Colunasss</td>
                    <td width="80%">
                    <?PHP
                        $agrupadorHtml =
                            <<<EOF
                                <table>
                                    <tr valign="middle">
                                        <td>
                                            <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 260px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
                                        </td>
                                        <td>
                                            <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>

                                            <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                                            <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>

                                            <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
                                        </td>
                                        <td>
                                            <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 260px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
                                        </td>
                                        <td>
                                            <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                                            <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                                        </td>
                                    </tr>
                                </table>
                                <script type="text/javascript" language="javascript">
                                    limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
                                    limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
                                    {POVOAR_ORIGEM}
                                    {POVOAR_DESTINO}
                                    sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
                                </script>
EOF;

                        $agrupador = new Agrupador( 'formulario', $agrupadorHtml );
                        //exibe agrupador
                        $agrupador->setOrigem( 'naoAgrupador_'.TPENSSUP, null, $origem[TPENSSUP] );
                        $agrupador->setDestino( 'agrupador_'.TPENSSUP, null, $destino[TPENSSUP] );
                        $agrupador->exibir();
?>
                    </td>
                </tr>
                <tr id="coluna_2" style="display:none;">
                    <td class="SubTituloDireita" valign="top" width="20%">Colunas</td>
                    <td width="80%">
<?PHP
                        $agrupadorHtml =
                            <<<EOF
                                <table>
                                    <tr valign="middle">
                                        <td>
                                            <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 260px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
                                        </td>
                                        <td>
                                            <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>

                                            <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                                            <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>

                                            <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
                                        </td>
                                        <td>
                                            <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 260px; height: 120px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
                                        </td>
                                        <td>
                                            <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                                            <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                                        </td>
                                    </tr>
                                </table>
                                <script type="text/javascript" language="javascript">
                                    limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
                                    limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
                                    {POVOAR_ORIGEM}
                                    {POVOAR_DESTINO}
                                    sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
                                </script>
EOF;

                        $agrupador = new Agrupador( 'formulario', $agrupadorHtml );
                        //exibe agrupador
                        $agrupador->setOrigem( 'naoAgrupador_'.TPENSPROF, null, $origem[TPENSPROF] );
                        $agrupador->setDestino( 'agrupador_'.TPENSPROF, null, $destino[TPENSPROF] );
                        $agrupador->exibir();
?>
			</td>
		</tr>
                <tr>
                    <td class="SubTituloDireita" valign="top">Unidades da Federação</td>
                    <td>
                        <?php
                            $sqlComboEstado = "
                                select estuf as codigo, estdescricao as descricao
                                from territorios.estado
                                order by estdescricao
                            ";
                            $estuf = array(0 => array("codigo" => "", "descricao" => "Todas"));
                            combo_popup("estuf", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top">Municípios</td>
                    <td>
                        <?php
                            $sqlComboMunicipio = "
                                select muncod as codigo, estuf || ' - ' || mundescricao as descricao
                                from territorios.municipio
                                order by estuf, mundescricao
                            ";
                            $muncod = array(0 => array("codigo" => "", "descricao" => "Todas"));
                            combo_popup("muncod", $sqlComboMunicipio, "Município", "400x400", 0, array(), "", "S", false, false, 5, 400);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top">Unidades</td>
                    <td>
                        <select multiple="multiple" size="5" name="unidades[]" id="unidades" ondblclick="abrepopupUnidade();" class="CampoEstilo" style="width:400px;">
                            <option value="">Todas</option>
                        </select>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top">Existência</td>
                    <td>
                        <?
                            $sql = "SELECT exiid as codigo, exidsc as descricao FROM academico.existencia WHERE existatus='A'";
                            $db->monta_combo('exiid', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'exiid', '');
                        ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top">Situação</td>
                    <td><input type="radio" name="cmpsituacao" value="" <?= $cmpsituacao ? '' : 'checked' ?>> Todos <input type="radio" <?= $cmpsituacao == 'F' ? 'checked' : '' ?> name="cmpsituacao" value="F"> Funcionando <input type="radio" name="cmpsituacao" <?= $cmpsituacao == 'N' ? 'checked' : '' ?>  value="N"> Não Funcionando</td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top">Instalações</td>
                    <td><input type="radio" name="cmpinstalacao" value="" <?= $cmpinstalacao ? '' : 'checked' ?>> Todos <input type="radio" name="cmpinstalacao"  <?= $cmpinstalacao == 'P' ? 'checked' : '' ?>  value="P"> Provisória <input type="radio" <?= $cmpinstalacao == 'D' ? 'checked' : '' ?> name="cmpinstalacao" value="D"> Definitiva <input type="radio"  <?= $cmpinstalacao == 'S' ? 'checked' : '' ?>  name="cmpinstalacao" value="S"> Sem Instalações</td>
                </tr>
                <!--
                <tr>
                        <td class="SubTituloDireita" valign="top">Situação de Obras no Campus</td>
                        <td><input type="radio" name="cmpsituacaoobra" value="" checked> Todos <input type="radio" name="cmpsituacaoobra" value="L"> Licitação da Obra <input type="radio" name="cmpsituacaoobra" value="A"> Obras em Andamento <input type="radio" name="cmpsituacaoobra" value="C">  Obras Concluídas <input type="radio" name="cmpsituacaoobra" value="E">  Elaboração de Projetos  <input type="radio" name="cmpsituacaoobra" value="D">  Dominialidade de Imóvel</td>
                </tr>
                -->
                <tr bgcolor="#C0C0C0">
                    <td>&nbsp;</td>
                    <td>
                        <input type="submit" value="Gerar CSV" onclick="document.getElementById('exibir').value='relatorio_csv';exibirRelatorio();">
                        <input type="submit" value="Gerar XLS" onclick="document.getElementById('exibir').value='relatorio_xls';exibirRelatorio();">
                        <input type="submit" value="Gerar HTML" onclick="document.getElementById('exibir').value='relatorio_html';exibirRelatorio();">

                        <input type="button" value="Salvar Consulta" onclick="javascript:salvarConsultaEfetuada();" style="cursor: pointer;"/>
                    </td>
                </tr>
            </table>
            
            <div id="div_selecao" name="div_selecao">
                <br><b style="margin-left: 6%;">Selecione o tipo de relatório que deseja que seja gerado</b><br>
                <input type="radio" name="rd_relatorio" id="rd_relatorio" value="relatorio_html" onclick="document.getElementById('exibir').value='relatorio_html';escolhaRelatorio();">Relatório em HTML <br/>
                <input type="radio" name="rd_relatorio" id="rd_relatorio" value="relatorio_xls" onclick="document.getElementById('exibir').value='relatorio_xls';escolhaRelatorio();">Relatório em XLS<br/>
                <input type="radio" name="rd_relatorio" id="rd_relatorio" value="relatorio_csv" onclick="document.getElementById('exibir').value='relatorio_csv';escolhaRelatorio();">Relatório em CSV
            </div>
    </form>

<script>
    showColunas('2');
</script>

<?php
    if ($_REQUEST['painel']){        
        echo "
            <script>
                $(document).ready(function (){
                    moveAllOptions(  document.getElementById( 'agrupador_2' ), document.getElementById( 'naoAgrupador_2' ), true, '');
                    $('#agrupador_2').append(new Option('Instituto', 'unidadeorc', true, true));
                    $('#agrupador_2').append(new Option('Campus', 'entnome', true, true));
                    $('#agrupador_2').append(new Option('Existência', 'exidsc', true, true));
                    $('#agrupador_2').append(new Option('Situação', 'cmpsituacao', true, true));
                    $('#agrupador_2').append(new Option('Instalações', 'cmpinstalacao', true, true));

                    document.getElementById('exibir').value='relatorio_html';
                    exibirRelatorio();
                });
            </script>
        ";
    }
?>