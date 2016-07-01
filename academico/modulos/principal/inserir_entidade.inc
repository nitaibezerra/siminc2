<?PHP
$orgid = $_SESSION['academico']['orgid'];

include_once APPRAIZ . "includes/classes/entidades.class.inc";

if($_SESSION['academico']['cmpid']){
        
    if($cmplocalidade){
        $sql = "
            SELECT
                loof.DS_LOGRADOURO,
                loof.NO_BAIRRO,
                loof.NO_LOCAL_OFERTA,
                loof.DS_COMPLEMENTO,
                mun.NO_MUNICIPIO,
                uf.NO_UF
            FROM ODSCENSOSUP_2012.DM_IES ies
            INNER JOIN ODSCENSOSUP_2012.DM_LOCAL_OFERTA_IES loof ON ies.CO_DM_IES = loof.CO_DM_IES
            INNER JOIN ODSCENSOSUP_2012.DM_CURSO curso ON loof.CO_DM_LOCAL_OFERTA_IES = curso.CO_DM_LOCAL_OFERTA_IES
            --INNER JOIN ODSCENSOSUP_2012.DM_MODALIDADE_ENSINO moda ON curso.CO_DM_MODALIDADE_ENSINO = moda.CO_DM_MODALIDADE_ENSINO
            INNER JOIN ODSCENSOSUP_2012.DM_MUNICIPIO mun ON (mun.CO_DM_MUNICIPIO = ies.CO_DM_MUNICIPIO)
            INNER JOIN ODSCENSOSUP_2012.DM_UF uf ON (mun.CO_DM_UF = ies.CO_DM_UF)
            WHERE ies.CO_CATEGORIA_ADMINISTRATIVA = 211 AND curso.CO_DM_NIVEL_ACADEMICO = 1 AND loof.CO_DM_LOCAL_OFERTA_IES = {$cmplocalidade}
            ";
        $localidadeCenso = $clsOracle->get($sql);
    }
}

if($_REQUEST['requisicaoAjax']){
    if($_REQUEST['requisicaoAjax'] == 'municipioAjax'){
        if($_POST['id']){
            $sql = "
                SELECT  CO_MUNICIPIO AS CODIGO,
                        NO_MUNICIPIO_EXIBICAO AS DESCRICAO
                FROM ODSCENSOSUP_2012.DM_MUNICIPIO m
                LEFT JOIN ODSCENSOSUP_2012.DM_UF uf ON (uf.CO_DM_UF = m.CO_DM_UF)
                WHERE SGL_UF = UPPER('{$_POST['id']}')
                ORDER BY NO_MUNICIPIO_EXIBICAO
            ";
            //$municipioData = $clsOracle->getAll($sql);
        }
    ?>
        <select name="CO_MUNICIPIO" id="CO_MUNICIPIO" data-placeholder="Selecione...">
            <option VALUE="">Selecione...</option>
            <?PHP if($municipioData): foreach($municipioData as $municipio): ?>
                <option <?PHP if($_POST['CO_MUNICIPIO'] == $municipio['CODIGO']) echo 'selected="selected"' ?> value="<?PHP echo $municipio['CODIGO']; ?>" ><?PHP echo $municipio['DESCRICAO'] ?></option>
            <?PHP endforeach;  endif;?>
        </select>
    <?PHP
        exit;

    }elseif($_REQUEST['requisicaoAjax'] == 'salvarCenso'){
        $sql = "
            SELECT  c.cmplocalidade
            FROM academico.campus c
            INNER JOIN entidade.entidade e ON e.entid = c.entid
            WHERE c.cmplocalidade = {$_POST['id']}
        ";
        $cmplocalidade = $db->pegaUm($sql);

        if($cmplocalidade){
            echo 'Não pode salvar, pois esta localidade já está atribuida para a instituição: ' + $entnome;
        } else {
            if(isset($_POST['id']) && !empty($_POST['id'])){
                $sql = "
                    UPDATE academico.campus
                        SET cmplocalidade = {$_POST['id']}
                    WHERE cmpid = {$_SESSION['academico']['cmpid']};
                ";
                $cmplocalidade = $db->executar($sql);
                $db->commit();
                echo 'Salvo com sucesso!';
            } else {
                echo 'É preciso selecionar uma localidade!';
            }
        }
        exit;

    }elseif($_REQUEST['requisicaoAjax'] == 'validarCenso'){
        if(isset($_POST['id']) && !empty($_POST['id'])){
            $sql = "
                SELECT entnome FROM academico.campus c
                INNER JOIN entidade.entidade e ON e.entid = c.entid
                WHERE c.cmplocalidade = {$_POST['id']}
            ";
            $entnome = $db->pegaUm($sql);
            if($entnome){
                echo 'Localidade já atribuida para a instituição: <b>' . $entnome . '<//b>';
            }
        }
        exit;

    }elseif($_REQUEST['requisicaoAjax']){
        $clsOracle = new ConnectOracle();

        $ufData = $clsOracle->getAll('SELECT SGL_UF AS CODIGO , SGL_UF AS DESCRICAO FROM ODSCENSOSUP_2012.DM_UF ORDER BY SGL_UF ');

        if(!isset($_POST['pesquisar']) && !empty($_SESSION['academico']['estuf'])) {
            $_POST['SGL_UF'] = str_to_upper($_SESSION['academico']['estuf']);
        }
        if($_POST){
            extract($_POST);
        }
        //ver($_POST['SGL_UF'], $ufData, d);
?>

        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
        <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="ISO-8895-1"></script>
        <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>
        <link href="../library/sliding_panel/css/slide.css" rel="stylesheet" media="screen">
        <script src="../library/chosen-1.0.0/chosen.jquery.js"></script>

        <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/base/jquery-ui.css'/>
        <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>
        <link rel='stylesheet' type='text/css' href='../library/jquery/jquery-ui-1.10.3/themes/bootstrap/jquery-ui-1.10.3.custom.min.css'/>
        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >

        <style>
            .chosen-container-single .chosen-single {
                display: block;
                 width: auto;
                height: 10px;
                padding: 8px 12px;
                font-size: 14px;
                line-height: 0.9;
                color: #666666;
                vertical-align: middle;
                background-color: #ffffff;
                border: 1px solid #cccccc;
                border-radius: 4px;
                -webkit-box-shadow: inset 0 1px 1px rgba(0, 0, 0, 0.075);
                box-shadow: inset 0 1px 1px rgba(0, 0, 0, 0.075);
                -webkit-transition: border-color ease-in-out 0.15s, box-shadow ease-in-out 0.15s;
                transition: border-color ease-in-out 0.15s, box-shadow ease-in-out 0.15s;
                text-decoration: none;
            }

            .chosen-container-single .chosen-single span {
                display: block;
                overflow: hidden;
                margin-right: 0px;
                text-overflow: ellipsis;
                white-space: nowrap;
            }
        </style>

        <script language="javascript">
            $1_11 = $.noConflict();
        </script>

        <form name="form_censo_pesquisa" method="post" action="/academico/academico.php?modulo=principal/inserir_entidade&acao=C&requisicaoAjax=localizarCenso">
            <table class="tabela" bgcolor="#f5f5f5" align="center">
                <input type="hidden" name="pesquisar" value="true" />
                <tr id="tr_titulo"><td class="SubTituloCentro" colspan="2">LOCALIDADE - CENSO</td></tr>
                <tr>
                    <td class="SubTituloDireita">Logradouro</td>
                    <td><?= campo_texto('DS_LOGRADOURO', 'N', 'S', 'Logradouro', 57, 50, '', '', '', '', '', 'id="DS_LOGRADOURO"', '', ''); ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Bairro</td>
                    <td><?= campo_texto('NO_BAIRRO', 'N', 'S', 'Bairro', 57, 50, '', '', '', '', '', 'id="NO_BAIRRO"', '', ''); ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Local de oferta</td>
                    <td><?= campo_texto('NO_LOCAL_OFERTA', 'N', 'S', 'Local de oferta', 57, 50, '', '', '', '', '', 'id="NO_LOCAL_OFERTA"', '', ''); ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">Complemento:</td>
                    <td><?= campo_texto('DS_COMPLEMENTO', 'N', 'S', 'Complemento', 57, 50, '', '', '', '', '', 'id="DS_COMPLEMENTO"', '', ''); ?></td>
                </tr>
                <tr>
                    <td class="SubTituloDireita">UF:</td>
                    <td>
                        <select name="SGL_UF" id="SGL_UF" data-placeholder="Selecione...">
                            <option value="">Selecione...</option>
                            <?PHP foreach($ufData as $uf): ?>
                                <option <?PHP if($_POST['SGL_UF'] == $uf['CODIGO']) echo 'selected="selected"' ?> value="<?PHP echo $uf['CODIGO']; ?>" ><?PHP echo $uf['DESCRICAO'] ?></option>
                            <?PHP endforeach; ?>
                        </select>
                    </td>
                </tr>
                <tr >
                    <td class="SubTituloDireita">Municipio:</td>
                    <td id="container_municipio">
                        <select name="CO_MUNICIPIO" id="CO_MUNICIPIO" data-placeholder="Selecione...">
                            <option value="">Selecione...</option>
                        </select>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita"></td>
                    <td><input type="submit" value="Procurar"></td>
                </tr>
            </table>
        </form>
    <?PHP
        $dataWhere = array();


        foreach($_POST as &$post){
            if($post){
               $post =  trim($post);
            }
        }

        if(!empty($_POST['DS_LOGRADOURO'])) $dataWhere[] = "UPPER(loof.DS_LOGRADOURO) LIKE UPPER('%{$_POST['DS_LOGRADOURO']}%')";
        if(!empty($_POST['NO_BAIRRO'])) $dataWhere[] = "UPPER(loof.NO_BAIRRO) LIKE UPPER('%{$_POST['NO_BAIRRO']}%')";
        if(!empty($_POST['NO_LOCAL_OFERTA'])) $dataWhere[] = "UPPER(loof.NO_LOCAL_OFERTA) LIKE UPPER('%{$_POST['NO_LOCAL_OFERTA']}%')";
        if(!empty($_POST['DS_COMPLEMENTO'])) $dataWhere[] = "UPPER(loof.DS_COMPLEMENTO) LIKE UPPER('%{$_POST['DS_COMPLEMENTO']}%')";
        if(!empty($_POST['CO_MUNICIPIO'])) $dataWhere[] = "mun.CO_MUNICIPIO = {$_POST['CO_MUNICIPIO']}";
        if(!empty($_POST['SGL_UF'])) $dataWhere[] = "uf.SGL_UF = UPPER('{$_POST['SGL_UF']}')";

        if( $dataWhere && count($dataWhere) > 0 ){
            $where = implode(' AND ' , $dataWhere );

            $sql = "
                SELECT DISTINCT
                    '<input name=\"local_censo\" type=\"radio\" value=\"' || loof.CO_DM_LOCAL_OFERTA_IES || '\" />' AS acao,
                    loof.DS_LOGRADOURO,
                    loof.NO_BAIRRO,
                    loof.NO_LOCAL_OFERTA,
                    loof.DS_COMPLEMENTO,
                    mun.NO_MUNICIPIO,
                    uf.NO_UF
                FROM ODSCENSOSUP_2012.DM_IES ies
                INNER JOIN ODSCENSOSUP_2012.DM_LOCAL_OFERTA_IES loof ON ies.CO_DM_IES = loof.CO_DM_IES
                INNER JOIN ODSCENSOSUP_2012.DM_CURSO curso ON loof.CO_DM_LOCAL_OFERTA_IES = curso.CO_DM_LOCAL_OFERTA_IES
                --INNER JOIN ODSCENSOSUP_2012.DM_MODALIDADE_ENSINO moda ON curso.CO_DM_MODALIDADE_ENSINO = moda.CO_DM_MODALIDADE_ENSINO
                INNER JOIN ODSCENSOSUP_2012.DM_MUNICIPIO mun ON (mun.CO_DM_MUNICIPIO = ies.CO_DM_MUNICIPIO)
                INNER JOIN ODSCENSOSUP_2012.DM_UF uf ON (uf.CO_DM_UF = ies.CO_DM_UF)

                WHERE ies.CO_CATEGORIA_ADMINISTRATIVA = 211 AND curso.CO_DM_NIVEL_ACADEMICO = 1 AND $where
            ";
            $result = $clsOracle->getAll($sql);
            $cabecalho = array('Ação' , 'Logradouro' , 'Bairro' , 'Local de oferta' , 'Complemento' , 'Município' , 'UF');
            $db->monta_lista_array($result , $cabecalho , 10 , 30 , 'S' , 'center');
?>
            <table style="width: 100%;">
                 <tr>
                     <td style="text-align: center;">
                         <input onclick="javascript:selecionarLocalCenso();" type="button" value="Salvar">
                     </td>
                 </tr>
             </table>
<?PHP
        }else{
             $where = '';
        }
?>

    <script language="JavaScript">

        $1_11('#SGL_UF').chosen();
        $1_11('#CO_MUNICIPIO').chosen();

        function selecionarLocalCenso(){
            if( $1_11('#modal') ){
                $1_11('#modal').remove();
            }

            $1_11('body').append('<div id="modal"></div>');

            if($1_11('input[name=local_censo]:checked').length == 0){
                $1_11('#modal').html('É preciso selecionar uma localidade!').dialog({
                    title: 'Aviso!',
                    position:{
                        my: "top",
                        at: "top",
                        of: window
                    },
                    buttons: [{
                        text: "Ok",
                        click: function(){
                            $1_11( this ).dialog( "close" );
                        }
                    }]
                });
            } else {
                var id = $1_11('input[name=local_censo]:checked').val();

                $1_11.post(
                    window.location.href,{
                        requisicaoAjax: 'salvarCenso' ,
                        id : id
                    }, function(html){
                        if( html != '' ){
                            $1_11('#modal').html(html).dialog({
                                title: 'Aviso!',
                                position: {
                                    my: "top",
                                    at: "top",
                                    of: window
                                },
                                buttons:[{
                                    text: "Ok",
                                    click: function(){
                                        $1_11( this ).dialog( "close" );
                                    }
                                }]
                        });
                            window.opener.location.reload();
                        }
                    });
            }
        }

        $1_11('input[name=local_censo]').change(function(){
            var id = $1_11(this).val();
            if($1_11('#modal')){
                $1_11('#modal').remove();
            }
            $1_11('body').append('<div id="modal"></div>');

            $1_11.post(window.location.href , {requisicaoAjax: 'validarCenso' , id : id} , function(html){
                if(html != ''){
                    $1_11('#modal').html(html).dialog({ title: 'Aviso!' , position: {my: "top", at: "top", of: window} , buttons: [ { text: "Ok", click: function() { $1_11( this ).dialog( "close" ); } } ] });
                    $1_11('input[name=local_censo]:checked').prop('checked',false);
                }
            } );
        });

        $1_11('#SGL_UF').change(function(){
            var id = $1_11(this).val();

            $1_11.post(window.location.href , {requisicaoAjax: 'municipioAjax' , id : id , CO_MUNICIPIO: 0} , function(html){
                $1_11('#container_municipio').hide().html(html).fadeIn();
                $1_11('#CO_MUNICIPIO').chosen();
            } );
        });

        if($1_11('#SGL_UF').val()){
            var id = $1_11('#SGL_UF').val();

            $1_11.post(window.location.href , {requisicaoAjax: 'municipioAjax' , id : id ,  CO_MUNICIPIO: '<?PHP echo $_POST['CO_MUNICIPIO'] ?>'} , function(html){
                $1_11('#container_municipio').hide().html(html).fadeIn();
                $1_11('#CO_MUNICIPIO').chosen();
            } );
        }
    </script>

<?PHP
        exit;
    }
}

#MONTA A CHAMADA DA FUNÇÃO COM A REQUISICAO. - USADO APENAS PELA TELA DADOS ESPECIFICOS DO CAMPOS.
if( $_REQUEST['requisicao'] ){
    $_REQUEST['requisicao']( $_REQUEST, $_FILES );
}
if($_REQUEST['exec_function']) {
    $_REQUEST['exec_function']($_REQUEST);
}elseif($_REQUEST['opt']=="buscarCnpj") {
    buscarCnpj($_REQUEST);
}

//Chamada de programa
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
echo "<br>";
?>

<script type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/entidades.js"></script>
<script type="text/javascript" src="./geral/js/academico.js"></script>

<script type="text/javascript">
    function abreSituacao( id ){
        if ( id != '2'){
            if (document.selection){
                if (document.getElementById("tr_situacao")){
                    document.getElementById("tr_situacao").style.display = 'block';
                }
            }else{
                if (document.getElementById("tr_situacao")){
                    document.getElementById("tr_situacao").style.display = 'table-row';
                }
            }
        }else{
            if (document.getElementById("tr_situacao")){
                document.getElementById("tr_situacao").style.display = 'none';
            }
            if (document.getElementById("cmpsituacaoobra")){
                document.getElementById("cmpsituacaoobra").value = '';
            }
        }
    }
</script>

<?PHP

    $acao = $_REQUEST['acao'];

    if ( $_REQUEST['acao'] == 'A' ){
        $_SESSION['sig_var']['entid'] = $_SESSION['academico']['entid'];
        $_SESSION['sig_var']['iscampus'] = 'nao';
        $_REQUEST['page'] = 'esp';

    }else if ( $_REQUEST['acao'] == 'C' ){
        $_SESSION['sig_var']['entid'] = $_SESSION['academico']['entidcampus'];
        $_SESSION['sig_var']['iscampus'] = 'sim';
        $_REQUEST['page'] = 'esp';
    }

    $orgid = $_SESSION['academico']['orgid'];

    if ($_SESSION['sig_var']['iscampus'] == 'nao'){
        $paramEntidade = 'entidunidade';
        $contexto	   = 'unidade';
    }else{
        $paramEntidade = 'entidcampus';
        $contexto	   = '';
    }

    if(!$_REQUEST['page']) {
        $_REQUEST['page'] = 'ent';
    }

    $perfilNotBloq = array(
        PERFIL_MECCADBOLSAS,
        PERFIL_MECCADCURSOS,
        PERFIL_MECCADASTRO,
        PERFIL_ADMINISTRADOR,
        PERFIL_CADASTROGERAL
    );

    #VERIFICANDO A SEGURANÇA
    $permissoes = verificaPerfilAcademico($perfilNotBloq);
    validaAcessoTipoEnsino($permissoes['vertipoensino'],$orgid);
    #FIM SEGURANÇA

    #CRIANDO REGRA ESPECIAL, QUANDO FOR A OPÇÃO "INSERIR CAMPUS/UNED", APRESENTAR A ABA DO PAI, OU SEJA, A ABA REFERENTE A LISTA DE UNED/CAMPUS
    switch($_REQUEST['page']) {
        case 'novoent':
            $pageparam = 'cam';
            break;
        default:
            $pageparam = $_REQUEST['page'];
    }
    
    if( $_SESSION['sig_var']['entid'] == '' ){
        $db->insucesso('Ocorreu um falha. Por favor, faça o login e tente novamente!', '', 'principal/inserir_entidade&acao=C');
        die();
    }

    #MONTA AS ABAS
    monta_titulo('Rede Federal', "Dados Especificos do Campus");
?>

<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
<?php
    $autoriazacaoconcursos = new autoriazacaoconcursos();
    // cria o cabeçalho padrão do módulo
    echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['sig_var']['entid']);
    #$db->cria_aba($abacod_tela,$url,$parametros);
    #$bloqueado = monta_cabecalho_academico($_SESSION['sig_var']['entid']);

    #VERIFICANDO EM QUAL PÁGINA O PROGRAMA DEVE IR
    switch($_REQUEST['page']) {
	#Tela das obras cadastradas (selecionar quais vão ser inauguradas)
	case 'obr':
        if($_SESSION['sig_var']['iscampus'] == 'sim') {
            $filtro = "e.entid='".$_SESSION['sig_var']['entid']."'";
        } elseif($_SESSION['sig_var']['iscampus'] == 'nao') {
            $filtro = "au.entid='".$_SESSION['sig_var']['entid']."'";
        } else {
            echo "<script>alert('Problemas com as variaveis de sessão.');window.location='?modulo=inicio&acao=C';<script>";
            exit;
        }

        $_SESSION['downloadfiles']['pasta'] = array("origem" => "obras","destino" => "sig");
?>
        <form action="?modulo=principal/inserir_entidade&acao=<?=$acao?>&exec_function=salvarObrasInaugurada" method="post" name="formulario" id="formulario">
            <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
                <tr>
                    <td class="SubTituloEsquerda">Obra(s) a ser(em) inaugurada(s)</td>
                </tr>
                <tr>
                    <td id="td_serinaugurada"></td>
                </tr>

                <script>
                    ajaxatualizar('exec_function=carregarlistaobras&buscar=serinaugurada','td_serinaugurada');
                </script>

<?PHP
                if ($permissoes['gravar']){
?>
                    <tr>
                        <td class="SubTituloDireita"><input type="button" value="Inserir/Remover obra a ser inaugurada" onclick="inserirobrainaugurada();"></td>
                    </tr>
<?PHP
                }else{
?>
                    <tr>
                        <td class="SubTituloDireita"><input type="button" value="Inserir/Remover obra a ser inaugurada" disabled="disabled"></td>
                    </tr>
<?PHP
                }
?>
                <tr>
                    <td class="SubTituloEsquerda">Outra(s) obra(s) do campus/uned</td>
                </tr>
                <tr>
                    <td id="td_outras"></td>
                </tr>

                <script>
                    ajaxatualizar('exec_function=carregarlistaobras&buscar=outras','td_outras');
                </script>

            </table>
        </form>
<?PHP
        break;
	#TELA DA LISTA DE CAMPUS (SOMENTE VISUALIZADA PARA UNIDADE ORÇAMENTÁRIA)
	case 'cam':
            $acao = "'<center><img src=\"/imagens/alterar.gif\" border=0 title=\"Editar\" style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editarcampus&acao=A&tpeid=".$orgid."&cmpid=' || cam.cmpid || '\'\");\">'";
            $campus = "'<a style=\"cursor:pointer;\" onclick=\"window.location=\'?modulo=principal/editarcampus&acao=A&tpeid=".$orgid."&cmpid=' || cam.cmpid || '\';\">'|| e.entnome||'</a>'";

            $sql = "
                SELECT  DISTINCT {$acao} as acao,
                        {$campus} as campus ,
                        en.estuf AS uf,
                        m.mundescricao AS municipio
                FROM entidade.entidade e

                INNER JOIN entidade.funcaoentidade fen ON fen.entid = e.entid
                INNER JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid
                INNER JOIN entidade.entidade au ON fea.entid = au.entid
                INNER JOIN entidade.funcaoentidade fen2 ON fen2.entid = au.entid
                INNER JOIN academico.orgaouo teu ON teu.funid = fen2.funid
                INNER JOIN academico.campus cam on e.entid = cam.entid

                LEFT JOIN entidade.endereco en ON e.entid = en.entid AND en.tpeid = 1
                LEFT JOIN territorios.municipio m on m.muncod = en.muncod

                WHERE fea.entid = '{$_SESSION['sig_var']['entid']}
            ";
            $cabecalho = array( "Ação", "Nome do campus/unidade", "UF", "Município");
            $db->monta_lista($sql,$cabecalho,50,5,'N','center',$par2);
?>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr bgcolor="#C0C0C0">
                    <td colspan="2">
                        <?PHP
                            if($permissoes['inserircampusuned']) {
                        ?>
                                <input type='button' onclick="window.location='?modulo=principal/inserir_entidade&acao=A&page=novoent';this.disabled=true" value='Inserir campus/uned'>
                        <? } ?>
                    </td>
		</tr>
            </table>
<?PHP
            break;

	case 'dir':
            if( $_SESSION['sig_var']['iscampus']=='sim' ){
                $funid_opc = $_funcoes[$orgid]['campus'];
            }else{
                $funid_opc = $_funcoes[$orgid]['unidade'];
            }
            $url = "academico.php?modulo=principal/inserir_entidade&acao={$acao}&exec_function=salvarRegistroDirigente";

            $entidade = new Entidades();
            $entidade->carregarPorFuncaoEntAssociado( $funid_opc, $_SESSION['sig_var']['entid'] );

            echo $entidade->formEntidade( $url,
                array( "funid" => $funid_opc, "entidassociado" => $_SESSION['sig_var']['entid'] ),
                array( "enderecos"=>array(1) )
            );
?>
            <script type="text/javascript">
		<? if(!$permissoes['gravar']) { ?>
                        $('btngravar').disabled = true;
		<? } ?>

		$('frmEntidade').onsubmit = function(e){

                    if(trim($F('entnumcpfcnpj')) == ''){
                        alert('CPF é obrigatório.');
                        return false;
                    }
                    if(trim($F('entnome')) == ''){
                        alert('O nome da entidade é obrigatório.');
                        return false;
                    }
                    if(trim($F('entdatanasc')) != ''){
                        if(!validaData(document.getElementById('entdatanasc'))) {
                            alert("Data de nascimento é inválida.");return false;
                        }
                    }
                    return true;
		}
            </script>
<?PHP
            break;
	case 'int':
            $entidade = new Entidades();
            $entidade->carregarPorFuncaoEntAssociado( INTERLOCUTORINS, $_SESSION['sig_var']['entid'] );
            $URL = "academico.php?modulo=principal/inserir_entidade&acao={$acao}&exec_function=salvarRegistroInterlocutor";

            echo $entidade->formEntidade( $url,
                array( "funid" => INTERLOCUTORINS, "entidassociado" => $_SESSION['sig_var']['entid'] ),
                array( "enderecos"=>array(1) )
            );

?>
            <script type="text/javascript">
		<? if(!$permissoes['gravar']) { ?>
                        $('btngravar').disabled=true;
		<? } ?>

		$('frmEntidade').onsubmit = function(e){
                    if(trim($F('entnumcpfcnpj')) == ''){
                        alert('CPF é obrigatório.');
                        return false;
                    }
                    if (trim($F('entnome')) == '') {
                        alert('O nome da entidade é obrigatório.');
                        return false;
                    }
                    if (trim($F('entdatanasc')) != '') {
                        if(!validaData(document.getElementById('entdatanasc'))) {
                            alert("Data de nascimento é inválida.");return false;
                        }
                    }
                    return true;
		}
            </script>
<?PHP
            break;
	case 'ent':
            if( $_SESSION['sig_var']['iscampus']=='sim' ){
                $funid_opc = $_funcoesentidade[$orgid]['campus'];
            }else{
                $funid_opc = $_funcoesentidade[$orgid]['unidade'];
            }

            $url = "academico.php?modulo=principal/inserir_entidade&acao={$acao}&exec_function=academico_salvarRegistroEntidade";

            $SQL = "
                SELECT  fea.entid
                FROM entidade.entidade ent
                LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid
                LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid

                WHERE fen.funid = '{$funid_opc}' AND fen.entid = '{$_SESSION['sig_var']['entid']}'
            ";
            $entidassociado = $db->pegaUm( $SQL );

            $entidade = new Entidades();
            $entidade->carregarPorEntid( $_SESSION['sig_var']['entid'] );
            echo $entidade->formEntidade( $url,
                array( "funid" => $funid_opc, "entidassociado" => $entidassociado ),
                array( "enderecos"=>array(1) )
            );
?>
            <script type="text/javascript">
		document.getElementById('tr_entcodent').style.display = 'none';
		document.getElementById('tr_entnuninsest').style.display = 'none';
		document.getElementById('tr_entungcod').style.display = 'none';
		document.getElementById('tr_tpctgid').style.display = 'none';

		/*
		 * DESABILITANDO O CAMPO DE CNPJ
		 */
		document.getElementById('entnumcpfcnpj').readOnly = true;
		document.getElementById('entnumcpfcnpj').className = 'disabled';
		document.getElementById('entnumcpfcnpj').onfocus = "";
		document.getElementById('entnumcpfcnpj').onmouseout = "";
		document.getElementById('entnumcpfcnpj').onblur = "";
		document.getElementById('entnumcpfcnpj').onkeyup = "";

		/*
		 * DESABILITANDO O CÓDIGO DA UNIDADE
		 */
		document.getElementById('entunicod').readOnly = true;
		document.getElementById('entunicod').className = 'disabled';
		document.getElementById('entunicod').onfocus = "";
		document.getElementById('entunicod').onmouseout = "";
		document.getElementById('entunicod').onblur = "";
		document.getElementById('entunicod').onkeyup = "";

		/*
		 * DESABILITANDO O NOME DA ENTIDADE
		 */
		document.getElementById('entnome').readOnly = true;
		document.getElementById('entnome').className = 'disabled';
		document.getElementById('entnome').onfocus = "";
		document.getElementById('entnome').onmouseout = "";
		document.getElementById('entnome').onblur = "";
		document.getElementById('entnome').onkeyup = "";

		/*
		 * DESABILITANDO A SIGLA DA ENTIDADE
		 */
		document.getElementById('entsig').readOnly = true;
		document.getElementById('entsig').className = 'disabled';
		document.getElementById('entsig').onfocus = "";
		document.getElementById('entsig').onmouseout = "";
		document.getElementById('entsig').onblur = "";
		document.getElementById('entsig').onkeyup = "";
    	</script>
<?PHP
	break;
	case 'novoent':
            $url = "academico.php?modulo=principal/inserir_entidade&acao={$acao}&exec_function=salvarNovoCampusUned";

            $entidade = new Entidades();
            echo $entidade->formEntidade(
                    $url,
                    array( "funid" => $_funcoesentidade[$orgid]['campus'], "entidassociado" => $_SESSION['sig_var']['entid'] ),
                    array( "enderecos"=>array(1) )
            );
?>
            <script type="text/javascript">
		document.getElementById('tr_entcodent').style.display = 'none';
		document.getElementById('tr_entnuninsest').style.display = 'none';
		document.getElementById('tr_entungcod').style.display = 'none';
		document.getElementById('tr_tpctgid').style.display = 'none';

		$('frmEntidade').onsubmit  = function(e) {
                    if (trim($F('entnome')) == '') {
                        alert('O nome da entidade é obrigatório.');
                        return false;
                    }
                    return true;
		}
            </script>
<?PHP
    break;
    case 'esp':
        #TELA DE 'DADOS ESPECÍFICOS' REFERENTE  AOS CAMPUS
        $tipolocalidade = definirtipolocalidade($orgid);

        if($_SESSION['sig_var']['iscampus'] == 'sim'){
            $sql = "
                SELECT  cam.cmpid,
                        cam.cmpobs,
                        entd.edtdsc,
                        cam.cmpdataimplantacao,
                        cam.cmpdatainauguracao,
                        cam.cmpdtportaria,
                        exi.exiid,
                        cam.cmpsituacao,
                        cam.cmpsituacaoobra,
                        cam.cmpinstalacao,
                        cam.cptid,
                        cam.datacriacao,
                        cam.arqidportaria

                FROM academico.campus cam
                LEFT JOIN academico.entidadedetalhe entd ON entd.entid = cam.entid
                LEFT JOIN academico.existencia exi ON exi.exiid = cam.exiid
                WHERE cam.entid = '{$_SESSION['sig_var']['entid']}'
            ";
            $dadoscampus = $db->carregar($sql);

            if($dadoscampus){
                $dadoscampus = current($dadoscampus);
            }

            #COLOCANDO NA SESSAO O CMPID PARA SALVAR A LOCALIDADE CENSO.
            $_SESSION['academico']['cmpid'] = $dadoscampus['cmpid'];
?>
            <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
            <link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>
            
            <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
            <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
            
            <script type="text/javascript">
                function dowloadDocAnexoPortaria( arqid ){
                    document.getElementById('arqidportaria').value = arqid;
                    document.getElementById('requisicao').value = 'dowloadDocAnexoPortaria';
                    document.getElementById('frmEntidadeDetalhes').submit();
                }
    
                function excluirDocAnexoPortaria(arqid){
                    var confirma = confirm("Deseja realmente excluir o Arquivo?");
                    if( confirma ){
                        document.getElementById('arqidportaria').value = arqid;
                        document.getElementById('requisicao').value = 'excluirDocAnexoPortaria';
                        document.getElementById('frmEntidadeDetalhes').submit();
                    }
                }
    
                function desabilita_check(){
                    var inaugura = document.getElementsByName('inaugura');

                    for(var i = 0; i < inaugura.length; i++){
                        if(inaugura[i].checked){
                            var valor_check = inaugura[i].value;
                        }
                    }

                    if( valor_check == 'nao' ){
                        document.getElementById('cmpdatainauguracao').value='';
                        document.getElementById('cmpdatainauguracao').disabled=true;
                    }else{
                        document.getElementById('cmpdatainauguracao').disabled=false;
                    }
                }

                function validaForm(){
                    if( validardadosespecificos() ){
                        document.getElementById('requisicao').value = 'salvarRegistroDetalhes';
                        document.getElementById('frmEntidadeDetalhes').submit();
                    }else{
                        return false;
                    }
                }
            </script>

            <form action="" method="POST" name="frmEntidadeDetalhes" id="frmEntidadeDetalhes" enctype="multipart/form-data">
                <input type="hidden" name="cmpid" id="cmpid" value="<?=$dadoscampus["cmpid"];?>">
                <input type="hidden" name="requisicao" id="requisicao" value="">
                <input type="hidden" name="arqidportaria" id="arqidportaria" value="">

                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td colspan="2">
                            <strong>Dados específicos d<?=$tipolocalidade["artigo+nome"]?></strong>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data de criação :</td>
                        <td>
                            <?PHP
                                if( trim($dadoscampus['datacriacao']) ){ 
                                    $datacriacao = substr($dadoscampus['datacriacao'],4,2)."/".substr($dadoscampus['datacriacao'],0,4);
                                }
                                echo campo_texto('datacriacao', "N", "S", "Data de Criação", 11, 7, "##/####", "", '', '', 0, 'id="datacriacao"')."(mm/yyyy)";
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data do início das atividades :</td>
                        <td>
                            <?PHP
                                if(trim($dadoscampus['cmpdataimplantacao'])){
                                    $cmpdataimplantacao = substr($dadoscampus['cmpdataimplantacao'],4,2)."/".substr($dadoscampus['cmpdataimplantacao'],0,4);
                                }
                                echo campo_texto('cmpdataimplantacao', "N", "S", "Data de Implantação", 11, 7, "##/####", "", '', '', 0, 'id="cmpdataimplantacao"')."(mm/yyyy)";
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data da Portaria de Autorização de Funcionamento :</td>
                        <td>
                            <?PHP
                                $cmpdtportaria = $dadoscampus['cmpdtportaria'];
                                echo campo_data2('cmpdtportaria','N','S','','S','','', $cmpdtportaria, '', '', 'cmpdtportaria' )."(dd/mm/yyyy)";
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Documento da Portaria de Autorização :</td>
                        <td>
                            <?PHP
                                if( $dadoscampus["arqidportaria"] > 0  ){
                            ?>
                                <input type="file" name="arquivo" id="name" disabled="disabled">
                            <?PHP
                                }else{
                            ?>
                                <input type="file" name="arquivo" id="name">
                            <?PHP
                                } 
                            ?>
                        </td>
                    </tr>
                    <?PHP
                        if ($dadoscampus["cmpid"] != '') {
                    ?>
                            <tr>
                                <td class="SubTituloDireita" width="25%">Lista de Arquivos:</td>
                                <td colspan="2">
                                    <?PHP
                                        $acao = "
                                            <img border=\"0\" title=\"Apagar arquivo\" onclick=\"excluirDocAnexoPortaria('|| a.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
                                            <img border=\"0\" title=\"Baixar arquivo\" onclick=\"dowloadDocAnexoPortaria('|| a.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
                                        ";

                                        $desativado = "
                                            <img border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir_01.gif\" />
                                            <img border=\"0\" title=\"Baixar arquivo\" onclick=\"dowloadDocAnexoPortaria('|| a.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
                                        ";
                                        
                                        $sql = "
                                            SELECT  CASE WHEN u.usucpf = '{$_SESSION['usucpf']}'
                                                        THEN '{$acao}'
                                                        ELSE '{$desativado}'
                                                    END AS acao,
                                                            
                                                    arqdescricao,
                                                    arqtipo,
                                                    arqnome ||'.'||	arqextensao AS arquivo, 
                                                    u.usunome AS usuario,
                                                    to_char(arqdata, 'DD/MM/YYYY') AS data_inclusao
                                            FROM academico.campus c

                                            JOIN public.arquivo AS a ON a.arqid = c.arqidportaria
                                            JOIN seguranca.usuario AS u ON u.usucpf = a.usucpf

                                            WHERE cmpid = {$dadoscampus['cmpid']}
                                        ";//ver($sql, d);
                                        $cabecalho = Array("Ação", "Descrição",  "Tipo de Documento", "Nome do arquivo", "Usuário Responsável", "Data da Inclusão");
                                        //$whidth = Array('3%', '30%', '10%');
                                        $align  = Array('center', 'left', 'left', 'left', 'right');
                                        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');
                                    ?>
                                </td>
                            </tr>
                    <?PHP } ?>
                    
                    
                    <tr>
                        <td class="SubTituloDireita">Data de inauguração :</td>
                        <td>
                            <?PHP
                                $cmpdatainauguracao=formata_data($dadoscampus['cmpdatainauguracao']);

                                if( trim($dadoscampus['cmpdatainauguracao']) == '' ){
                                    $checked_nao = "checked";
                                    $checked_sim = "";
                                }else{
                                    $checked_sim = "checked";
                                    $checked_nao = "";
                                }
                            ?>

                            <input type="radio" id="check_n_inaugura" name="inaugura" value="nao" onclick="desabilita_check();" <?=$checked_nao;?> > Não inaugurado
                            <input type="radio" id="check_inaugura" name="inaugura" value="sim" onclick="desabilita_check();" <?=$checked_sim;?> > Inaugurado

                            <?PHP
                                echo campo_texto('cmpdatainauguracao', "N", "S", "Data de inauguração", 12, 10, "##/##/####", "", '', '', 0, 'id="cmpdatainauguracao"' )."(dd/mm/yyyy)";
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Existência do campus/uned :</td>
                        <td>
                            <?PHP
                                $sql = "
                                    SELECT  exiid as codigo,
                                            exidsc as descricao
                                    FROM academico.existencia
                                    WHERE existatus='A'
                                ";
                                $db->monta_combo('exiid', $sql, 'S', 'Selecione', 'abreSituacao', '', '', '200', 'S', 'exiid', '', $dadoscampus['exiid']);
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Situação do campus/uned :</td>
                        <td>
                            <?PHP
                                if( $dadoscampus['cmpsituacao']=='F' ){
                                    $check_f = "checked";
                                }else{
                                    $check_n = "checked";
                                }
                            ?>
                            <input type='radio' name='cmpsituacao' value='F' <?=$check_f;?>> Funcionando
                            <input type='radio' name='cmpsituacao' value='N' <?=$check_n;?>> Não Funcionando
                        </td>
                    </tr>
                    <tr id='trcmpinstalacao'>
                        <td class="SubTituloDireita">Instalações :</td>
                        <td>
                            <?PHP
                                if( $dadoscampus['cmpinstalacao'] == 'P' ){
                                    $check_p = "checked";
                                }elseif( $dadoscampus['cmpinstalacao'] == 'D' ){
                                    $check_d = "checked";
                                }elseif( $dadoscampus['cmpinstalacao'] == 'S' ){
                                    $check_s = "checked";
                                }else{
                                    $check_p = "";
                                    $check_d = "";
                                    $check_s = "";
                                }
                            ?>
                            <input type='radio' id='cmpinstalacaoP' name='cmpinstalacao' value='P' <?=$check_p?>> Instalações Provisórias
                            <input type='radio' id='cmpinstalacaoD' name='cmpinstalacao' value='D' <?=$check_d?>> Instalações Definitivas
                            <input type='radio' id='cmpinstalacaoS' name='cmpinstalacao' value='S' <?=$check_s?>> Sem Instalações
                        </td>
                    </tr>
                    <tr id="tr_obrascampus">
                        <td class="SubTituloDireita">Obras no campus :</td>
                        <td>
                            <?PHP
                                $sql = "
                                    SELECT count(*) AS obras
                                    FROM obras.obrainfraestrutura oi
                                    WHERE stoid = 1 AND obsstatus = 'A' AND entidcampus = {$_SESSION['academico']['entidcampus']};
                                ";
                                $campusobras = $db->pegaUm( $sql );

                                if ( !$campusobras ){
                                    $campusobras 	 = 'N';
                                    $bloqObrasCampus = 'N';
                                }else{
                                    $campusobras 	 = 'S';
                                    $bloqObrasCampus = 'S';
                                }
                                $arrDados = array(
                                    0 => array(
                                        "codigo" => "S",
                                        "descricao" => "Sim"
                                    ),
                                    1 => array(
                                        "codigo" => "N",
                                        "descricao" => "Não"
                                    )
                                );
                                $db->monta_combo('campusobras', $arrDados, "N", 'Selecione', '', '', '', '200', 'N');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Tipo :</td>
                        <td>
                            <?PHP
                                $cptid = $dadoscampus['cptid'];
                                $sql = "
                                    SELECT  cptid AS codigo,
                                            cptdsc AS descricao
                                    FROM academico.campustipo;
                                ";
                                $db->monta_combo('cptid', $sql, 'S', 'Selecione', '', '', '', '200', 'N');
                            ?>
                        </td>
                    </tr>
<?PHP
                if ($_SESSION['academico']['orgid'] == 2){
                    #VICTOR PEDIU PARA VOLTAR COM O CAMPO
?>
                    <tr id="tr_situacao" style="display:none;">
                        <td class="SubTituloDireita">Situação da obra no campus/uned :</td>
                        <td>
                            <?PHP
                                $cmpsituacaoobra=$dadoscampus['cmpsituacaoobra'];
                                $sql = array(
                                    0 => array("codigo" => "L", "descricao" => "Licitação de Obras"),
                                    1 =>array("codigo" => "A", "descricao" => "Obras em Andamento"),
                                    2 =>array("codigo" => "C", "descricao" => "Obras Concluídas"),
                                    3=>array("codigo" => "E", "descricao" => "Elaboração de Projetos"),
                                    4=>array("codigo" => "D", "descricao" => "Dominialidade de Imóvel")
                                );
                                $db->monta_combo('cmpsituacaoobra', $sql, 'S', 'Selecione', '', '', '', '200', 'N', 'cmpsituacaoobra');
                            ?>
                        </td>
                    </tr>
<?PHP
            }
?>
                    <tr>
                        <td class='SubTituloDireita' width='340'>Caracterização da Unidade (<font size=1>texto sucinto destacando informações importantes sobre a unidade. Incluir informações sobre sua data de criação e sua importância para a região, dentre outras</font>) :</td>
                        <td>
                            <?PHP
                               $edtdsc = $dadoscampus['edtdsc'];
                               echo campo_textarea( 'edtdsc', 'N', 'S', '', '70', '5', '1000');
                           ?>
                        </td>
                    </tr>
                   <tr>
                       <td class="SubTituloDireita">Informações adicionais :</td>
                       <td>
                           <?PHP
                               $cmpobs = $dadoscampus['cmpobs'];
                               echo campo_textarea( 'cmpobs', 'N', 'S', '', '70', '5', '1000');
                           ?>
                       </td>
                   </tr>
                   <!-- DEFINIR LOCALIDADE - CENSO -->
                   <tr id="tr_titulo">
                       <td class="SubTituloCentro" colspan="2">LOCALIDADE - CENSO</td>
                   </tr>
                   <tr>
                       <td class="SubTituloDireita"></td>
                       <td></td>
                   </tr>
                   <tr>
                       <td class="SubTituloDireita">Logradouro</td>
                       <td>
                           <?PHP echo $localidadeCenso['DS_LOGRADOURO'] ?>
                       </td>
                   </tr>
                   <tr>
                       <td class="SubTituloDireita">Bairro</td>
                       <td>
                           <?PHP echo $localidadeCenso['NO_BAIRRO'] ?>
                       </td>
                   </tr>
                   <tr>
                       <td class="SubTituloDireita">Local de oferta</td>
                       <td>
                           <?PHP echo $localidadeCenso['NO_LOCAL_OFERTA'] ?>
                       </td>
                   </tr>
                   <tr>
                       <td class="SubTituloDireita">Complemento:</td>
                       <td>
                           <?PHP echo $localidadeCenso['DS_COMPLEMENTO'] ?>
                       </td>
                   </tr>
                   <tr>
                       <td class="SubTituloDireita">Municipio:</td>
                       <td>
                           <?PHP echo $localidadeCenso['NO_MUNICIPIO'] ?>
                       </td>
                   </tr>
                   <tr>
                       <td class="SubTituloDireita">UF:</td>
                       <td>
                           <?PHP echo $localidadeCenso['NO_UF'] ?>
                       </td>
                   </tr>
                   <tr>
                       <td class="SubTituloDireita">Bairro</td>
                       <td>
                           <?PHP echo $localidadeCenso['NO_BAIRRO'] ?>
                       </td>
                   </tr>
                   <tr>
                       <td class="SubTituloDireita"></td>
                       <td><input onclick="javascript:localizarCenso();" type="button" value="Localizar"></td>
                   </tr>
                    <tr>
                        <td colspan='2' bgcolor="#C0C0C0" align='center'>
<?PHP
                        if ($permissoes['gravar']){
?>
                            <input type='button' onclick="validaForm();" value='Salvar dados específicos'>
<?PHP
                        }else{
?>
                            <input type='button' disabled="disabled" value='Salvar dados específicos'>                            
<?PHP
                        }
?>
                        </td>
                    </tr>
                </table>
            </form>
<?PHP
        #FIM - TELA de 'Dados específicos' referente  aos campus

        } elseif($_SESSION['sig_var']['iscampus'] == 'nao') {
            #TELA DE 'DADOS ESPECÍFICOS' REFERENTE  AS UNIDADES ORÇAMENTÁRIAS
            
            $sql = "
                SELECT  entd.edtdsc 
                FROM academico.entidadedetalhe entd 
                WHERE entd.entid = '{$_SESSION['sig_var']['entid']}'
            ";
            $dadoscampus = $db->carregar($sql);
?>
            <form id="frmEntidadeDetalhes" method="post" action="?modulo=principal/inserir_entidade&acao=<?=$acao?>&exec_function=salvarRegistroDetalhes">
                <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr>
                        <td colspan='2'>
                            <strong>Dados específicos da Instituição</strong>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Caracterização da Instituição (texto sucinto destacando informações importantes sobre a unidade. Incluir informações sobre sua data de criação e sua importância para a região, dentre outras) :</td>
                        <td width="60%">
                            <?PHP
                                $edtdsc = $dadoscampus['edtdsc'];
                                echo campo_textarea( 'edtdsc', 'N', 'S', '', '70', '10', '1000');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td colspan="2" bgcolor="#C0C0C0" align="center">
<?PHP
                        if( $permissoes['gravar'] ){
?>
                            <input type="button" onclick="document.getElementById('frmEntidadeDetalhes').submit();this.disabled=true;" value="Salvar">
<?PHP            
                        }else{
?>
                            <input type="button" disabled="disabled" value="Salvar">
<?PHP 
                        } 
?>
                        </td>
                    </tr>
                </table>
            </form>
<?PHP      
        #FIM - TELA de 'Dados específicos' referente  as unidades orçamentárias
        }
	break;
    }# - FIM CASE

    if($bloqueado) {
        echo "<script>document.getElementById('btngravar').disabled=true</script>";
    }
    if ( $dadoscampus['exiid'] != '2'){
        echo "
            <script>
                if (document.selection){
                    document.getElementById(\"tr_situacao\").style.display = 'block';
                }else{
                    document.getElementById(\"tr_situacao\").style.display = 'table-row';
                }
            </script>
        ";
    }
?>

<?PHP
    $arrPerfil = array( PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR );
    $permissao = academico_possui_perfil($arrPerfil);
?>
<?PHP if($orgid == 2 && !$permissao ): ?>
    <script language="javascript">
//        jQuery('#exiid').attr('disabled','disabled');
//        jQuery('input[name=cmpsituacao]').attr('disabled','disabled');
//        jQuery('select[name=cptid]').attr('disabled','disabled');
        jQuery('#frmEntidadeDetalhes').find('input , select , textarea').attr('disabled','disabled');
    </script>
<?PHP endif; ?>

<script language="JavaScript">

    function localizarCenso()
    {
        console.info('teste');
//        if($1_11('#modal')){
//            $1_11('#modal').remove();
//        }
//        $1_11('body').append('<div id="modal"></div>');

        window.open(window.location.href + '&requisicaoAjax=localizarCenso' , "_blank", "width=1000, height=600");

//        $1_11.post(window.location.href , {requisicaoAjax: 'localizarCensu'} , function(html){
//            $1_11('#modal').html(html).dialog({ position: {my: "top", at: "top", of: window} });
//        } );
    }

</script>

</div>

  </body>
</html>
