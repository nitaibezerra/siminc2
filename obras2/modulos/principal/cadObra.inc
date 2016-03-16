<?php
verificaSessao('orgao');

$orgid = $_SESSION['obras2']['orgid'];
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = (int) ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];


$obridVinculado = $_REQUEST['obridVinculado'];
if ($obridVinculado) {
    $obraVinculada = new Obras($obridVinculado);
    $esdid         = pegaEstadoObra($obraVinculada->docid);

    if ($esdid != ESDID_OBJ_PARALISADO) {
        die("<script>
                    location.href = '?modulo=principal/cadObra&acao=A&obrid=" . $obridVinculado . "'
             </script>");
    }
    
    $_SESSION['obras2']['empid'] = $obraVinculada->empid;
    $_SESSION['obras2']['obrid'] = '';
}

switch ($_REQUEST['requisicao']) {
    case 'obshistoricoparalisacao':
        require_once(APPRAIZ . "obras2/modulos/principal/obshistorico.inc");
        die;
}

if($_REQUEST['requisicao'] == 'inserirrmarcacaoorgcontrole') {

        $obra = new Obras($obrid);
        $obra->obrorgcontrole = 't';
        $obra->salvar();
}
if($_REQUEST['requisicao'] == 'retirarmarcacaoorgcontrole') {
    $obra = new Obras($obrid);
    $obra->obrorgcontrole = 'f';
    $obra->salvar();
}
if($_REQUEST['requisicao'] == 'inserirrmarcacaocontabloqueada') {

    $obra = new Obras($obrid);
    $obra->obrcontabloqueada = 't';
    $obra->salvar();
}
if($_REQUEST['requisicao'] == 'retirarmarcacaocontabloqueada') {
    $obra = new Obras($obrid);
    $obra->obrcontabloqueada = 'f';
    $obra->salvar();
}

if($_REQUEST['requisicao'] == 'inserirrmarcacaoprocessoanterior') {
    $obra = new Obras($obrid);
    $obra->obrprocessoanterior = 't';
    $obra->salvar();
}
if($_REQUEST['requisicao'] == 'retirarmarcacaoprocessoaterior') {
    $obra = new Obras($obrid);
    $obra->obrprocessoanterior = 'f';
    $obra->salvar();
}


function habilitaDesbloqueio($esfera) {
    global $db, $obridVinculado;
    $obrid = ($_SESSION['obras2']['obrid'] ? $_SESSION['obras2']['obrid'] : $obridVinculado );

    if (!$obrid || !$esfera)
        return false;

    $sql = "
            SELECT count(*)
              FROM obras2.vm_pendencia_obras o
              WHERE o.empesfera = '$esfera' AND o.obrid = {$obrid}
              GROUP BY o.obrid
         ";

    $qtd = $db->pegaUm($sql);
    return $qtd > 0 ? true : false;
}

function removeAnexo() {
    global $db;

    $obra = new Obras();
    $obra->excluirImagem($_POST['obrid'], $_POST['arqid']);
    $db->commit();
    echo "true";

//	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
//	$file = new FilesSimec('arqsituacaodominial','arqid','obras');
//	if($file->setRemoveUpload($_REQUEST['arqid'])){
//		echo "true";
//	}
    die;
}

function salvar() {

    global $db;

    if (!$_SESSION['obras2']['empid'] || $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
        $empreendimento = new Empreendimento($_SESSION['obras2']['empid']);

        $endid = $empreendimento->endid;
        // Não permite ATUALIZAÇÃO de endereço, só INSERÇÃO
        if (!$empreendimento->endid) {
            $endereco                 = new Endereco($empreendimento->endid);
            $dadosEnd                 = $_POST['endereco'];
            $dadosEnd['tpeid']        = TIPO_ENDERECO_OBJETO;
            $dadosEnd['endcep']       = str_replace(Array('.', '-'), '', $_POST['endereco']['endcep']);
            $dadosEnd['medlatitude']  = $_POST['graulatitude'] . "." . $_POST['minlatitude'] . "." . $_POST['seglatitude'] . ".S";
            $dadosEnd['medlongitude'] = $_POST['graulongitude'] . "." . $_POST['minlongitude'] . "." . $_POST['seglongitude'] . ".W";
            $endid = $endereco->popularDadosObjeto($dadosEnd)
                              ->salvar();
        }
        
//        
        
        $dados                       = $_POST['empreendimento'];
        $dados['prfid']              = ($dados['prfid'] ? $dados['prfid'] : null);
        $dados['orgid']              = $_SESSION['obras2']['orgid'];
        $dados['tpoid']              = $_POST['obra']['tpoid'];
        $dados['tobid']              = $_POST['obra']['tobid'];
        $dados['cloid']              = $_POST['obra']['cloid'];
        $dados['endid']              = $_POST['obra']['endid'];
        $dados['empdsc']             = $_POST['obra']['obrnome'];
        $dados['empcomposicao']      = $_POST['obra']['obrcomposicao'];
        $dados['empjustsitdominial'] = $_POST['obra']['obrjustsitdominial'];
        $dados['empvalorprevisto']   = ($_POST['obra']['obrvalorprevisto'] ? MoedaToBd($_POST['obra']['obrvalorprevisto']) : NULL);
        $dados['usucpf']             = $_SESSION['usucpf'];
        $dados['entidunidade']       = $dados['entid'];
        $dados['endid']              = $endid;
        $empid                       = $empreendimento->popularDadosObjeto($dados)
                                                      ->salvar();

        $_POST['obra']['empid'] = $empid;
    }

    $obrid = $_SESSION['obras2']['obrid'];
    $obra  = new Obras($obrid);

    $arquivo = $_FILES["UploadPhoto"];
    $arqid = "";
    if ($_FILES["UploadPhoto"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
        if ($obrid && $_REQUEST['arqid']){
            $obra->excluirImagem($obrid, $_REQUEST['arqid']);
        }

        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

        $file = new FilesSimec();
        $file->setPasta('obras2');
        $file->setUpload(null, null, false);
        $arqid = $file->getIdArquivo();
    }

    $endid = $obra->endid;
    // Não permite ATUALIZAÇÃO de endereço, só INSERÇÃO
    if (!$obra->endid) {
        $endereco                 = new Endereco($obra->endid);
        $dadosEnd                 = $_POST['endereco'];
        $dadosEnd['tpeid']        = TIPO_ENDERECO_OBJETO;
        $dadosEnd['endcep']       = str_replace(Array('.', '-'), '', $_POST['endereco']['endcep']);
        $dadosEnd['medlatitude']  = $_POST['graulatitude'] . "." . $_POST['minlatitude'] . "." . $_POST['seglatitude'] . ".S";
        $dadosEnd['medlongitude'] = $_POST['graulongitude'] . "." . $_POST['minlongitude'] . "." . $_POST['seglongitude'] . ".W";
        $endid = $endereco->popularDadosObjeto($dadosEnd)
                          ->salvar();
    }

    $dados = $_POST['obra'];

    if ($dados['obridvinculado']) {
        $obraVinculada    = new Obras($dados['obridvinculado']);
        $dados['preid']   = ($obraVinculada->preid ? $obraVinculada->preid : null);
        $dados['obrid_1'] = ($obraVinculada->obrid_1 ? $obraVinculada->obrid_1 : null);
        $dados['tooid']   = $obraVinculada->tooid;
        $dados['obrnumprocessoconv']   = $obraVinculada->obrnumprocessoconv;
        $dados['obranoconvenio']   = $obraVinculada->obranoconvenio;
        $dados['numconvenio']   = $obraVinculada->numconvenio;
    }

/*	$dados['obrdtinicio'] = formata_data_sql($dados['obrdtinicio']);
	$dados['obrdtfim']    = formata_data_sql($dados['obrdtfim']);*/
    
    if(empty($obrid)){
        $dados['obrdtinclusao']  = 'now()';
        $dados['usucpfinclusao'] = $_SESSION['usucpf'];
    }
    
    $dados['endid']                   = $endid;
    $dados['obrvalorprevisto']        = ($dados['obrvalorprevisto'] ? MoedaToBd($dados['obrvalorprevisto']) : NULL);
    $dados['obrperccontratoanterior'] = ($dados['obridvinculado'] ? MoedaToBd($dados['obrperccontratoanterior']) : NULL);
    $dados['usucpf']                  = $_SESSION['usucpf'];
    $dados['arqid']                   = ($arqid ? $arqid : $dados['arqid']);
    $dados['obridvinculado']          = ($dados['obridvinculado'] ? $dados['obridvinculado'] : NULL);
    
    $obrid = $obra->popularDadosObjeto($dados)
                  ->salvar();

    if ($dados['obridvinculado']) {
//		$obraVinculada = new Obras( $dados['obridvinculado'] );
        $docid = pegaDocidObra($obrid);
        $obraVinculada->obrstatus = 'P';
        $obraVinculada->salvar();

        if ($obraVinculada->preid) {
            $sql = "UPDATE obras.preobra SET obrid = {$obrid} WHERE preid = {$obraVinculada->preid} AND prestatus = 'A'";
            $db->executar($sql);
        }
        $db->executar("UPDATE workflow.documento SET esdid = " . ESDID_OBJ_LICITACAO . "WHERE docid = $docid");

        wf_alterarEstado($docid, AEDID_OBJ_LICITACAO_LICITACAO, '', array());


    }

    if (is_array($_POST['tpcid'])) {
        foreach ($_POST['tpcid'] as $entid => $id) {
            unset($contatosObra);
            $contatosObra  = new ContatosObra();
            $cont['entid'] = $entid;
            $cont['tpcid'] = $id;
            $cont['obrid'] = $obrid;
            $contatosObra->popularDadosObjeto($cont)
                         ->salvar();
            $db->commit();
        }
    }

/*	if( is_array( $_POST['usucpf'] ) ){
//		$obra->limparResponsaveisObra();
//		foreach( $_POST['usucpf'] as $pflcod => $cpf ){
//			unset($usuarioResponsabilidade);
//			$usuarioResponsabilidade = new UsuarioResponsabilidade();
//			$uresp['usucpf'] = $cpf;
//			$uresp['pflcod'] = $pflcod;
//			$uresp['obrid']  = $obrid;
//			$usuarioResponsabilidade->popularDadosObjeto( $uresp )
//								    ->salvar();
//			$db->commit();
//		}
//	}*/
    
    

    $db->commit();
    
    //Caso seja o cadastro de uma nova obra vinculada a uma obra 
    if ($dados['obridvinculado']) {
        $idObraNova     = $obrid;
        $idObraOriginal = $dados['obridvinculado'];                
        posAcaoCadastroObraVinculada($idObraNova, $idObraOriginal);
        $obrid = $idObraOriginal;
    }

    $_SESSION['obras2']['obrid'] = $obrid;
    die('<script>
                alert(\'Operação realizada com sucesso!\');
                location.href=\'?modulo=principal/cadObra&acao=A&obrid=' . $obrid . '\';
         </script>');
    
}

function litaPrograma() {

    global $db;

    $programaFonte = new ProgramaFonte();
    $sql = $programaFonte->listaCombo(Array('tpoid' => $_REQUEST['tpoid']));
    if ($sql[0]['codigo'] == '') {
        $db->monta_combo('empreendimento[prfid]', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'prfid', '', $obra->tpoid, '');
    } else {
        $db->monta_combo('empreendimento[prfid]', $sql, 'S', 'Selecione...', '', '', '', 200, 'S', 'prfid', '', $obra->tpoid, '');
    }
}

if ($_REQUEST['req']) {
    $_REQUEST['req']();
    die();
}

// Inclusão de arquivos do componente de Entidade
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$_SESSION['obras2']['empid'] = ($_REQUEST['empid'] ? $_REQUEST['empid'] : $_SESSION['obras2']['empid']);

if ($_GET['acao'] != 'V') {
    //Chamada de programa
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";

//	$arMenuBlock = bloqueiaMenuObjetoPorSituacao( $obrid );
    $arMenuBlock = array();

    if (!$obrid && !$empid) {
        $db->cria_aba(ID_ABA_CADASTRA_OBRA_EMP, $url, $parametros, $arMenuBlock);
    } elseif ($obrid) {
        if ($orgid == ORGID_EDUCACAO_BASICA) {
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros, $arMenuBlock);
        } else {
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros, $arMenuBlock);
        }
    } else {
        $db->cria_aba(ID_ABA_CADASTRA_OBRA, $url, $parametros, $arMenuBlock);
    }

    $habilitado = true;
    $habilita = 'S';    
} else {
    ?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script>jQuery.noConflict();</script>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>


    <?php
    $_SESSION['obras2']['obrid'] = ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
    $db->cria_aba($abacod_tela, $url, $parametros);
//	$somenteLeitura = false;
    $habilitado = false;
    $habilita = 'N';
}

//if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL) ) ){
//	$habilitado = false;
//	$habilita = 'N';
//}
//demanda 311298 -- O perfil call center não pode alterar/inserir nenhuma informação.
if( possui_perfil( array(PFLCOD_CALL_CENTER) ) ){
	$habilitado = false;
	$habilita = 'N';
}
$obra                        = new Obras($_SESSION['obras2']['obrid']);
$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];
$empreendimento              = new Empreendimento($_SESSION['obras2']['empid']);
$empid                       = $empreendimento->empid;

if (empty($_SESSION['obras2']['obrid'])) {
    $dadoEmpreendimento = $empreendimento->getDados();
}

if ($obra->obrid) {
    $habilitado = false;
    $habilita = 'N';
}

$orgid             = $_SESSION['obras2']['orgid'];
$orgao             = new Orgao($orgid);
$tipologiaObra     = new TipologiaObra();
$programaFonte     = new ProgramaFonte();
$modalidadeEnsino  = new ModalidadeEnsino();
$tipoObra          = new TipoObra();
$classificacaoObra = new ClassificacaoObra();
$tipoOrigemObra    = new TipoOrigemObra();
$endereco          = new Endereco(($obra->endid ? $obra->endid : $dadoEmpreendimento['endid']));

//$orgid = $_SESSION['obras2']['orgid'];
$docid = pegaDocidObra($obrid);
if($obrid){
    echo cabecalhoObra($obrid);
}
//if ($empid) {
//    $empreendimento = new Empreendimento($empid);
//    $empreendimento->montaCabecalho();
//}

monta_titulo('Cadastro de Obra', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>jQuery.noConflict();</script>
<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>
<script src="/includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript">

    function inserirEntidade(entid, orgid) {
        if (entid) {
            if (confirm('Esta ação apagará todos os responsáveis da Obra. \n Deseja continuar?')) {
                jQuery('[id^="rpuid_"]').each(function() {
                    jQuery(this).remove();
                });
                return windowOpen('?modulo=principal/inserir_entidade&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&orgid=' + orgid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
            }
        } else {
            return windowOpen('?modulo=principal/inserir_entidade&acao=A&orgid=' + orgid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }

    function abreEdicaoEndereco() {
        windowOpen('?modulo=principal/editaEndereco&acao=O', 'blank', 'height=500,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function visualizarEntidade(entid, orgid) {
        if (entid) {
            return windowOpen('?modulo=principal/inserir_entidade&acao=A&busca=entnumcpfcnpj&bloq=disabled="disabled"&entid=' + entid + '&orgid=' + orgid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }

    function inserirContato(entid, tipo) {
        if (entid) {
            return windowOpen('?modulo=principal/selecionaContatoObra&acao=A&busca=entnumcpfcnpj&tipo=' + tipo + '&entid=' + entid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        } else {
            return windowOpen('?modulo=principal/selecionaContatoObra&acao=A&tipo=' + tipo, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }

    function inserirResponsavel(entid, tipo) {
        if (entid) {
            return windowOpen('?modulo=principal/selecionaResponsavelObra&acao=A&busca=entnumcpfcnpj&tipo=' + tipo + '&entid=' + entid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        } else {
            return windowOpen('?modulo=principal/selecionaResponsavelObra&acao=A&tipo=' + tipo, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }

    function visualizaContato(entid) {
        var index = window.document.getElementById('linha_' + entid).rowIndex;
        return windowOpen('?modulo=principal/selecionaContatoObra&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&tr=' + index + '&habilita=N', 'blank', 'height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function atualizaContato(entid) {
        var index = window.document.getElementById('linha_' + entid).rowIndex;
        return windowOpen('?modulo=principal/selecionaContatoObra&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&tr=' + index, 'blank', 'height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function RemoveLinha(idContato) {
        if (confirm('Deseja realmente excluir este contato?')) {
            var index = window.document.getElementById('linha_' + idContato).rowIndex;
            table = window.document.getElementById("contatos");
            table.deleteRow(index);
        }
    }

    function excluirResponsavel(rpuid) {
        if (confirm('Deseja realmente excluir este responsável?')) {
            var index = window.document.getElementById('rpuid_' + rpuid).rowIndex;
            table = window.document.getElementById("responsaveisobra");
            table.deleteRow(index);
        }
    }

    function atualizaPrograma(tpoid) {
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: "req=litaPrograma&tpoid=" + tpoid,
            async: false,
            success: function(msg) {
                jQuery('#td_programa').html(msg);
            }
        });
    }

    jQuery(document).ready(function() {

        jQuery('[type="text"]').keyup();
        jQuery('.salvar').click(function() {
            var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
            var stop = false;

            if (jQuery('#entid').val() == "") {
                mensagem += 'Unidade Implantadora \n';
                stop = true;
            }

            if (jQuery('#obrnome').val() == "") {
                mensagem += 'Nome da Obra \n';
                stop = true;
            }

            if (jQuery('#tpoid').val() == "") {
                mensagem += 'Tipologia da Obra \n';
                stop = true;
            }

            if (jQuery('#tobid').val() == "") {
                mensagem += 'Tipo da Obra \n';
                stop = true;
            }

            if (jQuery('#cloid').val() == "") {
                mensagem += 'Classificação da Obra \n';
                stop = true;
            }

            if (jQuery('[id*=obra[obrdsc]]').val() == "") {
                mensagem += 'Descrição / Composição da Obra \n';
                stop = true;
            }

            if (jQuery('#obrvalorprevisto').val() == "") {
                mensagem += 'Valor Previsto (R$) \n';
                stop = true;
            }

            <?php
                if ($obridVinculado):
            ?>
                        
                if (jQuery('#obrperccontratoanterior').val() == "") {
                    mensagem += 'Percentual executado aproveitável do contrato anterior (%)\n';
                    stop = true;
                }
                
            <?php
                endif;
            ?>

            if (jQuery('#empesfera').val() == "") {
                mensagem += 'Esfera \n';
                stop = true;
            }

            if (jQuery('#moeid').val() == "") {
                mensagem += 'Modalidade de Ensino \n';
                stop = true;
            }

            if (jQuery('#endcep').val() == "") {
                mensagem += 'CEP \n';
                stop = true;
            }

            if (stop) {
                alert(mensagem);
                return false;
            }

//		jQuery('.obrigatorio').each(function(){
//			if( jQuery(this).val() == '' ){
//				stop = true;
//				alert('Campo obrigatório.');
//				jQuery(this).focus();
//				return false;
//			}
//		});
            if (stop) {
                return false;
            }
            jQuery('#formObra').submit();
        });

        jQuery('#imgExcluir').click(function() {
            //obj = jQuery(this);
            if (confirm('Deseja excluir a foto?')) {
                jQuery.ajax({
                    url: '?modulo=principal/cadObra&acao=A',
                    type: 'post',
                    data: 'req=removeAnexo&obrid=' + jQuery('#obrid').val() + '&arqid=' + jQuery('#arqid').val(),
                    success: function(e) {
                        if (e == 'true') {
                            jQuery('#div_imagem').remove();
                            jQuery('#arqid').val('');
                        } else {
                            alert('Não foi possível deletar a foto!');
                        }
                    }
                });

            }
        });

    });

    function abrirContatosPar(obrid) {
        var horizontal = 1000;
        var vertical   = 600;

        var res_ver = screen.height;
        var res_hor = screen.width;

        var pos_ver_fin = (res_ver - vertical) / 2;
        var pos_hor_fin = (res_hor - horizontal) / 2;

        window.open('obras2.php?modulo=principal/popupContatosPar&acao=A&obrid=' + obrid, "Contatos PAR", "width=" + horizontal + ",height=" + vertical + ",top=" + pos_ver_fin + ",left=" + pos_hor_fin + ",status=yes");
    }

    function desbloqueioObras(obrid) {
        var horizontal = 1000;
        var vertical   = 700;

        var res_ver = screen.height;
        var res_hor = screen.width;

        var pos_ver_fin = (res_ver - vertical) / 2;
        var pos_hor_fin = (res_hor - horizontal) / 2;

        window.open('obras2.php?modulo=principal/pedidoDesbloqueio&acao=A&obrid=' + obrid, "Desbloqueio", "width=" + horizontal + ",height=" + vertical + ",top=" + pos_ver_fin + ",left=" + pos_hor_fin + ",status=yes");
    }
</script>
<form method="post" name="formObra" id="formObra" enctype="multipart/form-data">
    <input type="hidden" name="req" value="salvar"/>
    <input type="hidden" name="obra[obridvinculado]" id="obridvinculado" value="<?php echo $obridVinculado ?>"/>
    <input type="hidden" name="obra[obrid]" id="obrid" value="<?php echo $obra->obrid ?>"/>
    <input type="hidden" name="obra[empid]" value="<?php echo ($obra->empid ? $obra->empid : $_SESSION['obras2']['empid']) ?>"/>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
    <?php
        if ($obraVinculada->obrid):
    ?>
            <tr>
                <td align="justify" style="color: red;" colspan="2">
                    Você está cadastrando uma obra vinculado a obra paralisado, <b><?php echo '(' . $obraVinculada->obrid . ') ' . $obraVinculada->obrdsc ?></b>, este será desativado e não poderá mais sofrer alterações.
                </td>
            </tr>
    <?php
            $objChkLst = new ChecklistFnde();
            if($objChkLst->verificaBloqueioDeVinculoViaChecklistObraVinculada($obraVinculada->obrid)){
                $habilitado = false;
                $habilita = 'N';
                echo '<tr>
                        <td align="justify" style="color: red;" colspan="2">
                            A obra (' . $obraVinculada->obrid . ') ' . $obraVinculada->obrdsc.' possui pendências relativas ao Checklist de Obras Vinculadas.
                            Resolva essas pendências para proseguir com a criação da Obra Vinculada.
                        </td>
                    </tr>';
            }
        endif;
    ?>		
        <tr>
            <td class="SubTituloDireita" width="20%">Tipo de Ensino:</td>
            <td>
                <input type="hidden" name="orgid" value="<?php echo $_SESSION['obras2']['orgid'] ?>"/>
                <?php echo $orgao->orgdesc ?>
            </td>
            <td rowspan="8" align="right" valign="top" width="1">
            <?php
                // Barra de estado WORKFLOW
                if( possui_perfil(PFLCOD_CALL_CENTER)){
                    wf_desenhaBarraNavegacao( $docid, array('obrid' => $obrid), array('acoes' => true));
                }else if($docid){
                    wf_desenhaBarraNavegacao($docid, array('obrid' => $obrid));
                };
            ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="30%" valign="top">
                Foto:
            </td>	
            <td>
                <?php $display = ($obra->arqid ? 'display:block;' : 'display:none;'); ?>
                <div id="div_imagem" style="border:2px dashed #CCCCCC; padding:2px; margin-bottom:5px; width:350px; height:200px;<?php echo $display ?>">
                <?php //if($permissao_formulario == 'S'):  ?>
                    <img id="imgExcluir" src="../imagens/excluir_2.gif" style="position:absolute;cursor:pointer" title="Excluir Foto Atual"/>
                <?php //endif;  ?>
                <?php if ($obra->arqid): ?>
                    <img id="PhotoPrev" src="../slideshow/slideshow/verimagem.php?&arqid=<?php echo $obra->arqid; ?>&newwidth=350&newheight=200" style="width:350px;height:200px;"/>
                <?php endif; ?>
                </div>
                <input type="hidden" name="arqid" id="arqid" value="<?php echo $obra->arqid; ?>" />
                <?php if ($habilitado): ?>
                    <input type="file" name="UploadPhoto" id="UploadPhoto" size="55"/>
                <?php endif; ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Unidade Implantadora:</td>
            <td>
            <?php
            $entid_permitidas = obras_pegarUnidadesPermitidas();
            if (!$_SESSION['obras2']["obrid"]) {
                if ($entid_permitidas) {
                    $sql = "SELECT
                                e.entid as codigo,
                                e.entnome as descricao
                            FROM
                                entidade.entidade e
                            INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid
                            INNER JOIN obras2.orgaofuncao orf ON orf.funid = fue.funid
                            WHERE
                                e.entid in ( " . implode(", ", $entid_permitidas) . " ) AND orf.orgid = " . $_REQUEST["org"]
                            ;
                    $db->monta_combo("obra[entid]", $sql, 'S', "Selecione...", '', '', '', '340', 'S', 'entid');
                } else {
                    $entidade = new Entidade(($obra->entid ? $obra->entid : $dadoEmpreendimento['entidunidade']));
                    $entnome = $entidade->entnome;
                    $entid = $entidade->getPrimaryKey();
            ?>
                    <input type="hidden" name="obra[entid]" id="entid" value="<?php echo $entid; ?>" class="obrigatorio"> 
            <?php
                }
            } else {
                $entidade = new Entidade($obra->entid);
                $entnome = $entidade->entnome;
                $entid = $entidade->getPrimaryKey();
            ?> 
                <input type="hidden" name="obra[entid]" id="entid" value="<?php if (isset($_SESSION['obras2']["empid"])) echo $entid; ?>" class="obrigatorio"> 
            <?php
            }
            ?> 
            <span id="entnome">
                <a onclick="visualizarEntidade(document.getElementById('entid').value,<?php echo $orgid ?>);" style="cursor:pointer">
                    <?php echo $entnome; ?>
                </a>
                <input type="hidden" name="entnome_h" id="entnome_h" value="<?php echo $entnome; ?>">
           </span>
           <?php
                if ($habilitado && !$entid_permitidas) {
           ?> 
                    <input type="button" name="pesquisar_entidade" value="Pesquisar" style="cursor: pointer;"
                           onclick="inserirEntidade(document.getElementById('entid').value,<?php echo $orgid ?>);" <?php if ($habilitado == false) echo "disabled"; ?>> 
                    <img src="../imagens/obrig.gif" title="Indica campo obrigatório." border="0"> 
           <?php
                }
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome da Obra:</td>
            <td><?php echo campo_texto('obra[obrnome]', 'S', $habilita, '', 70, 100, '', '', '', '', '', 'id="obrnome"', '', ($obra->obrnome ? $obra->obrnome : $dadoEmpreendimento['empdsc'])); ?></td>
        </tr>
        <?php
            if ($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL):
        ?>
                <tr>
                    <td class="SubTituloDireita">Tipologia da Obra:</td>
                    <td>
                        <?php
                        $sql = $tipologiaObra->listaCombo(array('orgid' => $orgid));
                        $db->monta_combo('obra[tpoid]', $sql, $habilita, 'Selecione...', '', '', '', null, 'S', 'tpoid', '', ($obra->tpoid ? $obra->tpoid : $dadoEmpreendimento['tpoid']), '');
                        ?>
                    </td>
                </tr>
        <?php
            endif;
        ?>
        <tr>
            <td class="SubTituloDireita">Tipo da Obra:</td>
            <td>
                <?php
                    $sql = $tipoObra->listaCombo();
                    $db->monta_combo('obra[tobid]', $sql, $habilita, 'Selecione...', '', '', '', null, 'S', 'tobid', '', ($obra->tobid ? $obra->tobid : $dadoEmpreendimento['tobid']), '');
                ?>
            </td>
        </tr>
        <!-- 
                        <tr>
                                <td class="SubTituloDireita">Fonte:</td>
                                <td>
                <?php
//				$sql = $tipoOrigemObra->listaCombo();
//				$db->monta_combo('obra[tooid]',$sql, $habilita,'Selecione...','','','',200,'S', 'tooid', '', $obra->tooid, '');
                ?>
                                </td>
                        </tr>
        -->		
        <tr>
            <td class="SubTituloDireita">Classificação da Obra:</td>
            <td>
            <?php
                $sql = $classificacaoObra->listaCombo();
                $db->monta_combo('obra[cloid]', $sql, $habilita, 'Selecione...', '', '', '', null, 'S', 'cloid', '', ($obra->cloid ? $obra->cloid : $dadoEmpreendimento['cloid']), '');
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Descrição / Composição da Obra:</td>
            <td>
                <?php 
                    if($obridVinculado){
                        echo campo_textarea('obra[obrdsc]', 'N', $habilita, '', 100, 5, '', '', '', '', '', '', ($obra->obrdsc ? $obra->obrdsc : $dadoEmpreendimento['empcomposicao']), $param); 
                    }else{
                        echo campo_textarea('obra[obrdsc]', 'S', $habilita, '', 100, 5, '', '', '', '', '', '', ($obra->obrdsc ? $obra->obrdsc : $dadoEmpreendimento['empcomposicao']), $param);
                    }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Valor Previsto (R$):</td>
            <td colspan="2"><?php echo campo_texto('obra[obrvalorprevisto]', 'S', $habilita, '', 20, 20, '[.###],##', '', 'right', '', '', 'id="obrvalorprevisto"', '', ($obra->obrvalorprevisto ? $obra->obrvalorprevisto : $dadoEmpreendimento['empvalorprevisto'])); ?></td>
        </tr>
        <?php
            if ($obraVinculada->obrid || $obra->obridvinculado):
        ?>
            <tr>
                <td class="SubTituloDireita" style="">Percentual executado aproveitável do contrato anterior (%):</td>
                <td colspan="2"><?php echo campo_texto('obra[obrperccontratoanterior]', 'S', $habilita, '', 10, 5, '[.###],##', '', 'right', '', '', 'id="obrperccontratoanterior"', '', $obra->obrperccontratoanterior); ?></td>
            </tr>
        <?php
            endif;
        ?>		
        <!--  
        <tr>
                <td class="SubTituloDireita">Data Inicio:</td>
                <td colspan="2"><?php echo campo_data2('obra[obrdtinicio]', 'S', $habilita, '', '##/##/####', '', '', formata_data($obra->obrdtinicio), '', '', 'obrdtinicio'); ?></td>
        </tr>
        <tr>
                <td class="SubTituloDireita">Data Fim:</td>
                <td colspan="2"><?php echo campo_data2('obra[obrdtfim]', 'S', $habilita, '', '##/##/####', '', '', formata_data($obra->obrdtfim), '', '', 'obrdtfim'); ?></td>
        </tr>
        -->
        <?php if (!$_SESSION['obras2']['empid'] || $_SESSION['obras2']['orgid'] == 3) { ?>
            <tr>
                <td colspan="3">Dados da Obra</td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Esfera:</td>
                <td colspan="2">
                <?php
                    if ($orgid != ORGID_EDUCACAO_SUPERIOR) {
                        $sql = Array(
                            Array('codigo' => 'M', 'descricao' => 'Municipal'),
                            Array('codigo' => 'E', 'descricao' => 'Estadual')
                        );
                    } else {
                        $sql = Array(
                            Array('codigo' => 'M', 'descricao' => 'Municipal'),
                            Array('codigo' => 'E', 'descricao' => 'Estadual'),
                            Array('codigo' => 'F', 'descricao' => 'Federal')
                        );
                    }
                    $db->monta_combo('empreendimento[empesfera]', $sql, $habilita, 'Selecione...', '', '', '', 200, 'S', 'empesfera', '', $empreendimento->empesfera, '');
                ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Programa:</td>
                <td id="td_programa" colspan="2">
                    <?php

                 //   ver( $empreendimento,d);

                        $sql = $programaFonte->listaCombo(Array('tpoid' => $_REQUEST['tpoid']));
                        $msgTxt = ($empreendimento->prfid ? 'Selecione...' : 'Selecione uma tipologia antes...');
                        $db->monta_combo('empreendimento[prfid]', $sql, $habilita, $msgTxt, '', '', '', 200, 'N', 'prfid', '', $empreendimento->prfid, '');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Modalidade de Ensino:</td>
                <td colspan="2">
                    <?php
                        $sql = $modalidadeEnsino->listaCombo();
                        $db->monta_combo('empreendimento[moeid]', $sql, $habilita, 'Selecione...', '', '', '', 200, 'S', 'moeid', '', $empreendimento->moeid, '');
                    ?>
                </td>
            </tr>
        <?php } ?>
        <?php
            if ($obra->obrid):
        ?>
            <tr>
                <td colspan="3">Histórico de Paralisações</td>
            </tr>
            <tr>
                <td colspan="3">
                <?php
                    $histParalisacao = new HistoricoParalisacao();
                    $sql = $histParalisacao->listaSql(array('obrid' => $obra->obrid));
                    $cabecalho = array('Visualizar', 'Tipo de Paralisação', 'Data da Paralisação', 'Data da Liberação', 'Responsável');
                    $db->monta_lista_simples($sql, $cabecalho, 100, 30, 'N', '100%', 'S');
                ?>			
                </td>
            </tr>
        <?php
            endif;

            $habilitadoEnd = $habilitado;
            $habilitaEnd = $habilita;
            $classEnd = 'CampoEstilo';
            if ($obrid) {
                $habilitadoEnd = false;
                $habilitaEnd = 'N';
                $classEnd = 'disabled';
            }
                ?>
        <tr>
            <td colspan="3">Local da Obra</td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>
            <td colspan="2">
                <input type="text" name="endereco[endcep]" 
                       onkeyup="this.value = mascaraglobal('##.###-###', this.value);" 
                       onblur="Entidade.__getEnderecoPeloCEP(this);" class="<?php echo $classEnd; ?>" id="endcep" 
                       value="<?php echo $endereco->endcep ?>" size="13" maxlength="10" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?>/>
                <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
            </td>
        </tr>
        <tr id="escolha_logradouro_id" style="display:none">
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sugestão de Logradouro:</label></td>
            <td colspan="2">
                <input type="text" name="endlog" class="<?php echo $classEnd; ?>" id="endlog" value="<?php echo $endereco->endlog ?>" size="48" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                <label id="lbLogadouro"> Logradouro: </label>
            </td>
            <td colspan="2">
                <input type="text" name="endereco[endlog]" class="<?php echo $classEnd; ?>" id="endlogradouro" value="<?php echo $endereco->endlog ?>" size="65" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>
            <td colspan="2">
                <input type="text" name="endereco[endnum]" class="<?php echo $classEnd; ?>" id="endnum" value="<?php echo $endereco->endnum ?>" size="13" maxlength="8" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
            <td colspan="2">
                <input type="text" name="endereco[endcom]" class="<?php echo $classEnd; ?>" id="endcom" value="<?php echo $endereco->endcom ?>" size="65" maxlength="100" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
            <td colspan="2">
                <input type="text" name="endereco[endbai]" class="<?php echo $classEnd; ?>" id="endbai" value="<?php echo $endereco->endbai ?>" size="20" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>
            <td colspan="2">
                <input type="text" name="mundescricao"  class="<?php echo $classEnd; ?>" id="mundescricao" value="<?php echo $endereco->getMunDescricao() ?>" size="20" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
                <input type="hidden" name="endereco[muncod]" id="muncod" class="<?php echo $classEnd; ?>" value="<?php echo $endereco->muncod ?>" />
                <input type="text" name="endereco[estuf]" class="<?php echo $classEnd; ?>" id="estuf" value="<?php echo $endereco->estuf ?>" style="width: 5ex; padding-left: 2px" <?php echo ($habilitadoEnd == false ? 'disabled="disabled"' : '') ?> />
            </td>
        </tr>
        <tr>
            <td colspan="3">Coordenadas Geográficas</td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Latitude</td>
            <td colspan="2">
            <?php
                $medlatitude  = $endereco->medlatitude;
                $latitude     = explode(".", $medlatitude);
                $graulatitude = $latitude[0];
                $minlatitude  = $latitude[1];
                $seglatitude  = $latitude[2];
                $pololatitude = $latitude[3];
            ?> 
            <?php echo campo_texto('graulatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?> ° 
            <?php echo campo_texto('minlatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?> ' 
            <?php echo campo_texto('seglatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?> '' 
            <?php
                if (trim($pololatitude) == "S") {
                    echo "&nbsp;<span id=pololatitude_>S</span>";
                } elseif (trim($pololatitude) == "N") {
                    echo "&nbsp;<span id=pololatitude_>N</span>";
                } else {
                    echo "&nbsp;<span id=pololatitude_></span>";
                }
            ?>
            <input type="hidden" name="pololatitude" id="pololatitude" value="<?php echo trim($pololatitude) ?>">
            <?php 
                if(empty($obridVinculado)){
                    print obrigatorio(); 
                }
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Longitude</td>
            <td colspan="2">
            <?php
                $medlongitude  = $endereco->medlongitude;
                $longitude     = explode(".", $medlongitude);
                $graulongitude = $longitude[0];
                $minlongitude  = $longitude[1];
                $seglongitude  = $longitude[2];
                $pololongitude = $longitude[3];
            ?> 
            <?php echo campo_texto('graulongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?> ° 
            <?php echo campo_texto('minlongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?> ' 
            <?php echo campo_texto('seglongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"'); ?> ''
            <?php
                if (trim($pololongitude) == "W") {
                    echo "&nbsp;<span id=pololatitude_>W</span>";
                } elseif (trim($pololongitude) == "E") {
                    echo "&nbsp;<span id=pololatitude_>E</span>";
                } else {
                    echo "&nbsp;<span id=pololatitude_></span>";
                }
            ?>
            <input type="hidden" name="pololatitude" id="pololatitude" value="<?php echo trim($pololongitude) ?>">
            <?php 
                if(empty($obridVinculado)){
                    print obrigatorio(); 
                }
            ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"></td>
            <td colspan="2">
                <?php
                    if ($habilitadoEnd):
                ?>
                    <a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a>
                <?php
                    elseif (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC)) && !$obra->preid):
                ?>
                    <a href="#" onclick="abreEdicaoEndereco();">Editar Endereço da Obra</a>
                <?php
                    endif;
                ?>
                <input style="display: none;" 
                       type="text" 
                       name="endereco[endzoom]" 
                       id="endzoom"
                       value=<?php if ($endereco->endzoom == null) echo "15"; else echo $endereco->endzoom; ?>>
            </td>
        </tr>
        <!--  
        <tr>
                <td colspan="2">Sobre a Inauguração</td>
        </tr>
        <tr>
                <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Inaugurada:</label></td>
                <td>
                        <input type="radio" name="obra[obrstatusinauguracao]" value="S" <?php echo ($obra->obrstatusinauguracao == 'S' ? 'checked="checked"' : '' ) ?>/> Não se aplica <br>
                        <input type="radio" name="obra[obrstatusinauguracao]" value="N" <?php echo ($obra->obrstatusinauguracao == 'N' ? 'checked="checked"' : '' ) ?>/> Não Inaugurada <br>
                        <input type="radio" name="obra[obrstatusinauguracao]" value="I" <?php echo ($obra->obrstatusinauguracao == 'I' ? 'checked="checked"' : '' ) ?>/> Inaugurada
        </tr>
        -->


    <?php

     if($endereco->medlongitude){ ?>
    <tr>
        <td colspan="3">Localização geográfica</td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Mapa</td>
        <td colspan="2">
            <div id="map-canvas"></div>
        </td>
    </tr>
    <?php } ?>

        <tr>
            <td colspan="3" align="center" bgcolor="#dcdcdc"><b>Contatos</b></td>
        </tr>
        <tr>
            <td colspan="3">
            <?php
                $contato        = new Contato();
                $_POST['estuf'] = $endereco->estuf;
                $_POST['orgid'] = $orgid;
                $sql            = $contato->listaResumoSqlSemCPF($_POST);
                $cabecalho      = array("Nome", "E-mail", "Telefone");
                
                $db->monta_lista_simples($sql, $cabecalho, 30, 5, 'N');
            ?>
            </td>
        </tr>
        <tr>
            <td colspan="3" align="center" bgcolor="#dcdcdc"><b>Responsáveis pela Obra</b></td>
        </tr>
        <tr>
            <td colspan="3">
            <?php
                $contato   = new Contato();
                $sql       = $contato->ReponsavelObraEGestorUnidade($empid);
                $cabecalho = array("Nome", "E-mail", "Telefone", "Perfil",'Situação do cadastro');
//				$db->monta_lista($sql,$cabecalho,30,5,'N','center','');
                $db->monta_lista_simples($sql, $cabecalho, 30, 5, 'N');
            ?>
            </td>
        </tr>
        <tr>
            <td colspan="3" align="center" bgcolor="#dcdcdc"><b>Contatos do PAR</b></td>
        </tr>
        <tr>
            <td colspan="3"><a href="#" onclick="abrirContatosPar(<?php echo $_SESSION['obras2']['obrid'] ?>);"><img style="cursor:pointer;" src="/imagens/consultar.gif" border="0" title="Visualizar Contatos PAR"> Visualizar Contatos PAR</a></td>
        </tr>
        <?php
            if (habilitaDesbloqueio($empreendimento->empesfera) && (possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO))):
        ?>
                <tr>
                    <td colspan="3" align="center" bgcolor="#dcdcdc"><b>Desbloqueio</b></td>
                </tr>
                <tr>
                    <td colspan="3">
                        <a href="#" onclick="desbloqueioObras(<?php echo $_SESSION['obras2']['obrid'] ?>);">
                            <img style="cursor:pointer;" src="/imagens/consultar.gif" border="0" title="Desbloqueio de Obras"> Desbloqueio de Obras
                        </a>
                    </td>
                </tr>
        <?php
            endif;
        ?>
        <tr>
            <td style="background-color:#DCDCDC" colspan="3" align="center">
            <?php if ($habilitado): ?>
                    <input type="button" name="salvar" class="salvar" value="Salvar"/>
            <?php endif; ?>
            </td>
        </tr>
    </table>
</form>

<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formObra'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                   }
                 ";
            echo '</script>';
        }
?>
<script src="https://maps.googleapis.com/maps/api/js?v=3.exp"></script>

<?php



?>


<script>

    function coordToDec(coord){

            var match, dec, s, d, m,direcao;
            match = coord.split(".");

            d = match[0];
            m = match[1];
            s = match[2];
            direcao = match[3];
            //alert(direcao);
            dec = Math.sign(d) * (Math.abs(d) + (m / 60.0) + (s / 3600.0));
            if(direcao == 'S' || direcao == 'W' || direcao !== 'undefined') {
                dec = dec * -1;
            }

            return dec;

         }



    function initialize() {
        <?php
        if(empty($endereco->medlatitude))
        {
        $municipios = $db->pegaLinha("select  CASE WHEN (length (munmedlat)=8) THEN
								                CASE WHEN length(REPLACE('0' || munmedlat,'S','')) = 8 THEN
								                    ((SUBSTR(REPLACE('0' || munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'S',''),1,2)::double precision))*(-1)
								                ELSE
								                    (SUBSTR(REPLACE('0' || munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE('0' || munmedlat,'N',''),1,2)::double precision)
								                END
								            ELSE
								                CASE WHEN length(REPLACE(munmedlat,'S','')) = 8 THEN
								                   ((SUBSTR(REPLACE(munmedlat,'S',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'S',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'S',''),1,2)::double precision))*(-1)
								                ELSE
								                  0--((SUBSTR(REPLACE(munmedlat,'N',''),5,4)::double precision/3600000)+(SUBSTR(REPLACE(munmedlat,'N',''),3,2)::double precision/60)+(SUBSTR(REPLACE(munmedlat,'N',''),1,2)::double precision))
								               END
								            END  as latitude, (SUBSTR(REPLACE(munmedlog,'W',''),1,2)::double precision + (SUBSTR(REPLACE(munmedlog,'W',''),3,2)::double precision/60)) *(-1) as longitude  from territorios.municipio where muncod ='{$endereco->muncod}'");

        ?>
        var myLatlng = new google.maps.LatLng(<?php echo $municipios['latitude']?>,<?php echo $municipios['longitude']?>);
        <?php
        }else{
         ?>
        var myLatlng = new google.maps.LatLng(coordToDec("<?php echo simec_trim($endereco->medlatitude)?>"),coordToDec("<?php echo simec_trim($endereco->medlongitude)?>"));


        <?php } ?>
        var mapOptions = {
            zoom: 12,
            center: myLatlng
        };


        map = new google.maps.Map(document.getElementById('map-canvas'),
            mapOptions);

        var marker = new google.maps.Marker({
            position: myLatlng,
            map: map,
            title: 'Obra!'
        });

    }

    google.maps.event.addDomListener(window, 'load', initialize);


</script>

<style>
    #map-canvas {
        height: 300px;
        width: 100%;
        margin: 0px;
        padding: 0px
    }
</style>