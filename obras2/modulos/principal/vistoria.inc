<?php
// empreendimento || obra || orgao
verificaSessao( 'obra' );
unset($_SESSION['supid']);

$habilitaMods = false;

require_once APPRAIZ . "includes/classes/fileSimec.class.inc";

switch ($_POST['op']) {
    case 'alterar':
        $_SESSION['obras2']['smiid'] = $_POST['smiid'];
        header('Location: ?modulo=principal/cadSupervisaoMI&acao=E');
        die;
    case 'apagar':
        $supervisaoMi = new SupervisaoMi( $_POST['smiid'] );
        $supervisaoMi->smistatus = 'I';
        $supervisaoMi->salvar();
        $db->commit();
}

function salvarArquivosArt(){
	global $db;
        
        $obrid = $_REQUEST['obrid'];
	
        $art_exec = NULL;
        $art_fisc = NULL;

     //1 - Execução
	$campos	= array("obrid"	  => $obrid,
                        "oardesc" => "'".$_POST['oardesc1']."'",
                        "tpaid"   => $_POST['tpaid1'],
                        "oardata" => " now() ");
	
	$file = new FilesSimec("obras_arquivos", $campos, 'obras2');
	if($_FILES["arquivo1"]){	
            $arquivoSalvo = $file->setUpload($_POST['oardesc1'],"arquivo1", TRUE, "oarid");
            if($arquivoSalvo){
                $arqid_exec = $file->getIdArquivo();

                $oarid = $file->getCampoRetorno();
                $art_exec = TRUE;

                $arrArquivosArt['usucpf'] = $_POST['usucpf_fiscal'];
                $arrArquivosArt['entidempresa']= $_POST['empresa_contratada_id'];
                if(empty($arrArquivosArt['entidempresa'])){$arrArquivosArt['entidempresa'] = null;}
                //ver('aaa',d);
                $arrArquivosArt['oarid'] = $oarid;
                $FiscalregAtividade                 = new FiscalRegistroAtividade();
                $FiscalregAtividade->popularDadosObjeto($arrArquivosArt);
                $FiscalregAtividade->salvar();
            }else{
                $arqid_exec = NULL;
                $art_exec = FALSE;                
            }
	}
        
        unset($file);
        unset($campos);
        unset($arquivoSalvo);
        
        //2 - Fiscalização
        $campos	= array("obrid"	  => $obrid,
                        "oardesc" => "'".$_POST['oardesc2']."'",
                        "tpaid"   => $_POST['tpaid2'],
                        "oardata" => " now() ");
	
	$file = new FilesSimec("obras_arquivos", $campos, 'obras2');
	if($_FILES["arquivo2"]){	
            $arquivoSalvo = $file->setUpload($_POST['oardesc2'],"arquivo2", TRUE, "oarid");
            if($arquivoSalvo){
                $arqid_fisc = $file->getIdArquivo();
                $oarid = $file->getCampoRetorno();
                $art_fisc   = TRUE;

                $arrArquivosArt['usucpf'] = $_POST['usucpf_fiscal'];
                $arrArquivosArt['entidempresa']= $_POST['empresa_contratada_id'];

                if(empty($arrArquivosArt['entidempresa'])){$arrArquivosArt['entidempresa'] = null;}
                $arrArquivosArt['oarid'] = $oarid;
                $FiscalregAtividade                 = new FiscalRegistroAtividade();
                $FiscalregAtividade->popularDadosObjeto($arrArquivosArt);
                $FiscalregAtividade->salvar();
            }else{
                $arqid_fisc = NULL;
                $art_fisc   = FALSE;                
            }
	}
        
        //Registro de Atividades
        $arrArquivosArt['arquivo_art_exec'] = $arqid_exec;
        $arrArquivosArt['arquivo_art_fisc'] = $arqid_fisc;



        $arrArquivosArt['arquivo_art_fisc'] = $arqid_fisc;
        $arrArquivosArt['arquivo_art_fisc'] = $arqid_fisc;

    $supervisao = new Supervisao();
        $supervisao->registroDeMudancaDeArquivoArt($arrArquivosArt, $obrid);
        $supervisao->commit();
        
        if($art_exec == TRUE && $art_fisc == TRUE){
            echo '<script type="text/javascript"> 
                        alert("Arquivos anexados com sucesso.");
                        document.location.href = document.location.href;
                  </script>';
        }elseif($art_exec == FALSE && $art_fisc == TRUE){
            echo '<script type="text/javascript"> 
                        alert("Erro ao fazer o upload do arquivo ART de Execução. Arquivo ART de Fiscalização anexado com sucesso.");
                        document.location.href = document.location.href;
                  </script>';
        }elseif($art_exec == TRUE && $art_fisc == FALSE){
            echo '<script type="text/javascript"> 
                        alert("Erro ao fazer o upload do arquivo ART de Fiscalização. Arquivo ART de Execução anexado com sucesso.");
                        document.location.href = document.location.href;
                  </script>';            
        }
}

function downloadArquivosArt(){
    
	$obraArquivo = new ObrasArquivos();
	$arDados = $obraArquivo->buscaDadosPorArqid( $_REQUEST['arqid'] );
	$eschema = ($arDados[0]['obrid_1'] ? 'obras' : 'obras2');
	
	$file = new FilesSimec(null,null,$eschema);
	$file->getDownloadArquivo( $_REQUEST['arqid'] );
	
	die('<script type="text/javascript">
			document.location.href = document.location.href;
		  </script>');
}

function excluirArquivosArt(){
    
    $obrid = $_REQUEST['obrid'];
    
    if(possui_perfil(Array(PFLCOD_GESTOR_UNIDADE,PFLCOD_SUPERVISOR_UNIDADE))){
        echo "<script type=\"text/javascript\">
                    alert('Você não tem permissões suficientes para excluir esse documento!');
                    document.location.href = document.location.href;
              </script>";
    }else{
	
	global $db;
        
	$obrasArquivos	= new ObrasArquivos( $_REQUEST['oarid'] );
        
        if($obrasArquivos->tpaid == 25){
            $arrArquivosArt['arquivo_art_exec'] = $obrasArquivos->arqid;
            $arrArquivosArt['arquivo_art_fisc'] = NULL;
        }elseif($obrasArquivos->tpaid == 26){
            $arrArquivosArt['arquivo_art_exec'] = NULL;
            $arrArquivosArt['arquivo_art_fisc'] = $obrasArquivos->arqid;
        }
        
        $supervisao = new Supervisao();
        $supervisao->registroDeMudancaDeArquivoArt($arrArquivosArt, $obrid);
        
	$obrasArquivos->popularDadosObjeto( Array('oarstatus' => 'I') )
		      ->salvar();
	$obrasArquivos->commit();
	
	echo "<script type=\"text/javascript\">
			alert('Operação realizada com sucesso!');
			document.location.href = document.location.href;
		  </script>";
    }
}

if(isset($_REQUEST['supid']) || isset($_POST['sueid']) ){
   
    // Caso Supervisão Instituição
    if(isset($_REQUEST['supid'])){
        $vistoria = new Supervisao();
        $obrid = $_SESSION['obras2']['obrid']; 
        $supid = $_REQUEST['supid'];

        $dadosVistoria = $vistoria->getSupervisao($obrid, $supid);
        $dataCalc      = isset($dadosVistoria[0]["dtinclusao"]) ? $dadosVistoria[0]["dtinclusao"] : date('Y-m-d');
        
        $dadosValidade = $vistoria->getValidadeSupUnidade($obrid, $supid);
        $validado      = (strtolower($dadosValidade[0]["validadaPeloSupervisorUnidade"]) == 's') ? true : false;        
    }
    // Caso Supervisão Empresa
    elseif(isset($_POST['sueid'])){
        $vistoria = new SupervisaoEmpresa();
        $obrid    = $_SESSION['obras2']['obrid']; 
        $sueid    = $_POST['sueid'];
        
        $dadosVistoria = $vistoria->listaDados(array('sueid'=>$sueid));        
        $dataCalc      = isset($dadosVistoria[0]["suedtcadastro"]) ? $dadosVistoria[0]["suedtcadastro"] : date('Y-m-d');        
        $validado      = (strtolower($dadosVistoria[0]["esddsc"]) == 'homologado') ? true : false;
    }

    //RN - Não permitir a edição/exclusão de vistorias pelo supervisor unidade ou gestor unidade 7 dias após a data de inclusão.
    $dataInicial  = explode('/',$dataCalc);
    $dataInicial  = $dataInicial[2].'-'.$dataInicial[1].'-'.$dataInicial[0];
    $dataFinal    = date('Y-m-d');
    $diffDatas    = ((((strtotime($dataFinal) - strtotime($dataInicial))/60)/60)/24);
    $habilitaMods = ( ($diffDatas >= 7) && (possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE)) && ($validado == true) ) ? false : true;
    
}

// para post do formulario empresa
switch ( $_POST['operacao'] ){
    case 'apagarSupervisaoEmpresa':
        if($habilitaMods){
            $supervisaoEmpresa = new SupervisaoEmpresa();
            $supervisaoEmpresa->excluir( $_POST['sueid'] );
            $db->commit();
            die("<script>
                    alert('Operação realizada com sucesso!');
                    window.location = '?modulo=principal/listVistoriaEmp&acao=A';
                 </script>");
        }else{
            die("<script>
                     alert('Você não possui permissão para excluir esse Acompanhamento!');
                     history.back(-1);
                  </script>");
        }
    case 'editarSupervisaoEmpresa':

        if($habilitaMods){
            $_SESSION['obras2']['sueid'] = $_POST['sueid'];
            die("<script>
                    window.location = '?modulo=principal/cadVistoriaEmpresa&acao=E';
                 </script>");                
        }else{
            die("<script>
                     alert('Você não possui permissão para editar esse Acompanhamento!');
                     history.back(-1);
                  </script>");
        }            
    case 'alterarSupervisaoFNDE':
        if($habilitaMods){
            $_SESSION['obras2']['sfndeid'] = $_POST['sfndeid'];
            header('Location: ?modulo=principal/cadSupervisaoFNDE&acao=E');
            die;
        }else{
            die("<script>
                     alert('Você não possui permissão para alterar esse Acompanhamento!');
                     history.back(-1);
                  </script>");
        }
    case 'apagarSupervisaoFNDE':
        if($habilitaMods){
            $supervisaoFnde = new SupervisaoFNDE( $_POST['sfndeid'] );
            $supervisaoFnde->sfndestatus = 'I';
            $supervisaoFnde->salvar();

            $db->commit();
            die("<script>
                    alert('Operação realizada com sucesso!');
                    location.href = '?modulo=principal/listaSupervisaoFNDE&acao=A';
                 </script>");
        }else{
            die("<script>
                     alert('Você não possui permissão para alterar esse Acompanhamento!');
                     history.back(-1);
                  </script>");
        }
}
    
// Post do formulario de Arquivo ART
if( $_REQUEST['req'] ){
    $_REQUEST['req']();
    die();
}

$caminho_atual = 'obras2.php?modulo=principal/vistoria&';

$obrid = $_SESSION['obras2']['obrid'];

/*
//// captura dados do projeto da requisição
//$atiid = $_REQUEST['atiid'] ? (integer) $_REQUEST['atiid'] : PROJETO;
// $_SESSION["atiid"] = $atiid;
//$obrid = $_SESSION['obras2']['obrid'];
//setado para teste
//$habilitado = true;
//if ( empty($obrid) ){
//    die("<script>
//            alert('Faltam parametros para acessar esta tela!');
//            history.go(-1);
//         </script>");
//}
*/

ini_set( "memory_limit", "100M" );

include APPRAIZ . 'includes/Agrupador.php';
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

// @TODO Verificar para adaptar
// Executa as funções da tela

$vistoria = new Supervisao();

$responsaveisPossiveis = $vistoria->resgataTodosRsuidsPossiveis();

if( $_REQUEST["subacao"] == "VerificaVistoria" && $habilitaMods === true){
    
    // Verifica se existe vistorias com data maior do que a da que esta tentando excluir
    $param		 = array();
    $param['not(emsid)'] = true;
    $param['not(smiid)'] = true;
    $pode_excluir_vistoria = $vistoria->verificaExistenciaVistorias( $_REQUEST["supid"], $param );

    $habilita = $vistoria->podeatualizarvistoria( $_REQUEST["supid"] );
    
    if( $pode_excluir_vistoria && $habilita == 'S'){

        $boExcluida = $vistoria->deletarVistoria( $_REQUEST["supid"], $param );

        if ( true !== $boExcluida ){
            echo "<script>
	                 alert('" . $boExcluida . "');
	                 history.back(-1);
	              </script>";
        }

    }else{
        
        echo "<script>
                 alert('Você não possui permissão para excluir esse Acompanhamento!');
                 history.back(-1);
              </script>";

    }

}
elseif( $_REQUEST["subacao"] == "VerificaVistoria" && $habilitaMods === false){
    echo "<script>
            alert('Você não possui permissão para excluir esse Acompanhamento!');
            history.back(-1);
          </script>";    
}

$esdid = 0;
//$_GET['acao'] = 'V';
$obra  = new Obras( $obrid );
if( $_GET['acao'] != 'V' ){
	// Inclusão de arquivos padrão do sistema
	include APPRAIZ . 'includes/cabecalho.inc';
	// Cria as abas do módulo
	echo '<br>';
	if( $_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA ){
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE,$url,$parametros);
	}else{
		$db->cria_aba(ID_ABA_OBRA_CADASTRADA,$url,$parametros);
	}
	

	$esdid = pegaEstadoObra( $obra->docid );
	if ( $db->testa_superuser() || $esdid == ESDID_OBJ_EXECUCAO || $esdid == ESDID_OBJ_PARALISADO || $esdid == ESDID_OBJ_CONCLUIDO || $esdid == ESDID_OBJ_INACABADA ){
		$habilitado = true;
		$habilita   = 'S';
	}else{
		$habilitado = false;
		$habilita 	= 'N';
	}
}
else{
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<?php
	$db->cria_aba($abacod_tela,$url,$parametros);
//	criaAbaVisualizacaoObra();
//	$somenteLeitura = true;
	$habilitado = false;
	$habilita   = 'N';
}

if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CALL_CENTER, PFLCOD_CONSULTA_TIPO_DE_ENSINO) ) ){
	$habilitado = false;
	$habilita = 'N';
}

/**
 * Verifica se existe uma Trava no Cronograma. Caso o campo obras2.obras.obrtravaedicaocronograma = FALSE, deve-se bloquear as vistorias
 * para que o cronograma seja atualizado.
 */
$obr = new Obras($obrid);
$str_trava_cronograma_vistoria = '';
if($obr->getTravaCronograma($obrid) == 'f'){
    $habilitado = false;
    $habilita = 'N';
    $str_trava_cronograma_vistoria = 'A inclusão de nova vistoria está boqueada até que o Cronograma da Obra seja atualizado.';
}

// Colocar aqui uma restrição na edição dos Arquivos ART caso necessário.
if(possui_perfil(array(PFLCOD_CALL_CENTER))){
    $habilitado_art = false;
}else{
    $habilitado_art = true;
}

echo cabecalhoObra($obrid);

echo alertaObraMi($obrid);
/*
//$db->cria_aba($abacod_tela,$url,$parametros);
//$titulo_modulo = "Monitoramento de Obras/Infraestrutura";
//monta_titulo( $titulo_modulo, 'Acompanhamento da Obra' );
//
//cabecalho_projeto($atiid);
*/

?>
<script src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
    
    $(document).ready(function(){
        
	$('[type="text"]').keyup();
	$('.incluir_art').click(function(){
		var stop = false;
		$('.obrigatorio').each(function(){
			if( $(this).val() == '' ){
				stop = true;
				alert('Campo obrigatório.');
				$(this).focus();
				return false;
			}
		});
		if( stop ){
			return false;
		}
		$('#req').val('salvarArquivosArt');
		$('#formulario_art').submit();
	});
	$('.download').click(function(){
		$('#req').val('downloadArquivosArt');
		$('#arqid').val( $(this).attr('id') );
		$('#formulario_art').submit();
	});
	$('.excluir').click(function(){
		if ( confirm('Deseja apagar o documento?') ){
			$('#req').val('excluirArquivosArt');
			$('#oarid').val( $(this).attr('id') );
			$('#formulario_art').submit();
		}
	});
});

</script>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloDireita" width="20%">Mostrar vistorias realizadas por:</td>
        <td>
            <?php foreach ($responsaveisPossiveis as $responsavelPossivel) { 
                if( $responsavelPossivel['rsuid'] != 4 ){?>
                
                <span style="margin-left:5px">
                    <input type="checkbox" id="chamada_responsavel_<?php echo$responsavelPossivel['rsuid']?>" name="chamada_responsavel_<?php echo$responsavelPossivel['rsuid']?>" value="responsavel_<?php echo$responsavelPossivel['rsuid']?>" onclick="javascript:toggleParaResponsabilidades( this.value )" <?php echo($responsavelPossivel['rsuid']==1 || $responsavelPossivel['rsuid']==3)?'checked':''?>/>
                    <?php echo $responsavelPossivel['rsudsc'] ?></span>

            <?php }} ?>
                <span style="margin-left:5px">
                    <input type="checkbox" id="chamada_responsavel_4" name="chamada_responsavel_4" value="responsavel_4" onclick="javascript:toggleParaResponsabilidades(this.value)" <?php echo($_REQUEST['chamada_responsavel_4']==1)?'checked':''?>/>
                    Empresa MI
                </span>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="20%">Mostrar Documentos ART?</td>
        <td>
            <span style="margin-left:5px">
            <input type="checkbox" id="chamada_documentos_art" name="chamada_documentos_art" value="documentos_art" checked onclick="javascript:toggleParaResponsabilidades( this.value )" /> Mostrar
        </td>
    </tr>
    <?php
    $strSQL = "SELECT o.obrperccontratoanterior, 
                     (SELECT obrpercentultvistoria FROM obras2.obras WHERE obrid = o.obridvinculado) as obrpercentultvistoria_vinculado,
                      o.obridvinculado
               FROM obras2.obras as o
               WHERE obrid = {$obrid}";
    $dados_obra_vinculada = $db->pegaLinha($strSQL);
    if (!empty($dados_obra_vinculada['obridvinculado'])) {
    ?>
        <tr>
            <td class="SubTituloDireita" width="20%">Obra Vinculada:</td>
            <td>
                <span style="margin-left:5px">
                <input type="checkbox" id="chamada_obra_vinculada" onclick="javascript:toggleParaObraVinculada( jQuery(this).parents('table:first') )" /> Mostrar
                </span>
            </td>
        </tr>
    <?php
    }
    ?>
    
</table>
<div id="documentos_art" style="">
<?php
monta_titulo( 'Documentos ART', '' );
?>
<form id="formulario_art" name="formulario_art" method="post" enctype="multipart/form-data" action="?modulo=principal/vistoria&acao=A">
    <input type="hidden" id="obrid"    name="obrid"    value="<?php echo $obrid;?>" />
    <input type="hidden" id="operacao" name="operacao" value="arquivoArt" />
    <input type="hidden" id="req"      name="req"      value="" />
    <input type="hidden" id="arqid"    name="arqid"    value="" />
    <input type="hidden" id="oarid"    name="oarid"    value="" />
    
    <?php if( $habilitado_art ){ ?>
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">

                    <?php
                    $crtid    = $obra->pegaContratoPorObra( $obrid );
                    $contrato = new Contrato( $crtid );

                    $dados    = $contrato->getDados();
                    $empresa_contratada_id = $dados['entidempresa'];
                    $empresa 	= new Entidade( $empresa_contratada_id );
                    $empresa_contratada = $empresa->entnome;


                    ?>
                    <?php

                    $sql = "
                    SELECT
                        u.usunome, u.usucpf
                    FROM obras2.usuarioresponsabilidade ur
                    INNER JOIN seguranca.usuario u ON u.usucpf = ur.usucpf
                    INNER JOIN seguranca.usuario_sistema us ON us.usucpf = u.usucpf AND sisid = 147 AND us.susstatus = 'A' AND us.suscod = 'A'
                    INNER JOIN obras2.empreendimento e ON e.empid = {$obra->empid}
                    WHERE
                        ur.rpustatus = 'A' AND
                        u.suscod = 'A' AND
                        ur.pflcod = 948 AND
                        --u.usustatus = 'A' AND
                        ( (ur.empid = {$obra->empid}) )";
                    $dado = $db->pegaLinha($sql);
                    $fiscal_responsavel = $dado['usunome'];

                    ?>

                    <tr>
                        <td class="SubTituloDireita" width="20%">Empresa contratada:</td>
                        <td>
                            <?php if (obraMi($obra->obrid)){ $obrigacao_mi = "N";}else{$obrigacao_mi = "S";}?>
                            <?php echo campo_texto('empresa_contratada',$obrigacao_mi, "S",'',43,100,'','', '', '', '', 'readonly="readonly"', '');?>
                            <input type="hidden" name="empresa_contratada_id" value="<?php echo $empresa_contratada_id?>">
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="20%">Fiscal Responsável:</td>
                        <td>
                            <?php echo campo_texto('fiscal_responsavel','S', 'S','',43,100,'','', '', '', '', 'readonly="readonly"', '');?>
                            <input type="hidden" name="usucpf_fiscal" value="<?php echo $dado['usucpf']?>">
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="20%">Descrição do Anexo ART de Execução:</td>

                        <td><?php echo campo_texto('oardesc1','S', 'S','',43,100,'','', '', '', '', 'id="oardesc1"', '');?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Tipo:</td>
                        <td>
                        <?php
                            $tipoArquivo = new TipoArquivo();
                            $sql = $tipoArquivo->listaCombo(array('tpaid=25'));
                            $db->monta_combo("tpaid1", $sql, 'S', "", "", '', '', '', 'S', 'tpaid1','', 25);
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Arquivo ART de Execução Paga e Assinada:</td>
                        <td>
                            <input type="file" name="arquivo1" id="arquivo1" class="obrigatorio"/>
                            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" width="20%">Descrição do Anexo ART de Fiscalização:</td>
                        <td><?php echo campo_texto('oardesc2','S','S','',43,100,'','', '', '', '', 'id="oardesc2"', '');?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Tipo:</td>
                        <td>
                        <?php
                            $tipoArquivo = new TipoArquivo();
                            $sql = $tipoArquivo->listaCombo(array('tpaid=26'));
                            $db->monta_combo("tpaid2", $sql, 'S', "", "", '', '', '', 'S', 'tpaid2','', 26);
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Arquivo ART de Fiscalização Paga e Assinada:</td>
                        <td>
                            <input type="file" name="arquivo2" id="arquivo2" class="obrigatorio"/>
                            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                        </td>
                    </tr>
                    <tr>
                        <td style="background-color:#DCDCDC" colspan="3" align="center">
                            <?php if( $habilitado_art ){ ?>
                                <input type="button" name="salvar" class="incluir_art" value="Anexar Arquivo"/>
                            <?php }?>
                        </td>
                    </tr>
		</table>
    <?php 
    
        } 
    
    
        $obrasArquivos 	= new ObrasArquivos();
        
        $param = array("obrid" => $obrid/*, "not(img)" => true*/);
        
        if ( $habilitado_art == false ){
                $param['block_img_excluir'] = true;
        }
        
        $param['tpaid(IN)'] = '25,26';

        $sql = $obrasArquivos->listaSqlVistoria( $param );
        
//        ver($sql);

        $cabecalho 	= array("Ação11", "Tipo Arquivo", "Descrição", "Arquivo", "Data de Inclusão", "Gravado por","Fiscal","Empresa");
        $db->monta_lista($sql,$cabecalho,50,5,'N','center','', "formulario");
//        $db->monta_lista($sql,$cabecalho,50,5,'N','center',$par2, "formulario");

    ?>
    
</form>
</div>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td>
        <?php
        //ver($responsaveisPossiveis);
        foreach( $responsaveisPossiveis as $responsavelPossivel ){ 
            
            if( $responsavelPossivel['rsuid'] != 4 )
                monta_titulo( $responsavelPossivel['rsudsc'], '' );

            if( $responsavelPossivel['rsuid'] == 1 ){ // Instituição ############################################################################
                $param               = array();
                $param['not(emsid)'] = true;
                $param['not(smiid)'] = true;
                
                if( possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_MEC) ){
                    $dados = $vistoria->listaDadosGestorUnidade( $obrid, $param );
                }
                else{
                    $dados = $vistoria->listaDados( $obrid, $param );
                }

                $cabecalho = array( "Ação",
                                    //"Aditivo Vinculado",
                                    "Ordem",
                                    "ID",
                                    "Data Acompanhamento",
                                    "Data Inclusão",
                                    "Inserido Por",
                                    "Situação da Obra",
                                    "Responsável",
                                    "Realizada Por",
                                    "% do Acompanhamento");

                if( possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_MEC) )
                    array_push( $cabecalho, "Validada pelo Supervisor" );
                ?>

                <form name="formulario" id="formulario" method="post" onSubmit="return Validacao();" action="<?php echo $caminho_atual;?>acao=A">
                <input type="hidden" name="requisicao" id="requisicao" value="executar"/>

                <table class="tabela" align="center" style="width: 98%;<?php echo($responsavelPossivel['rsuid']!=1)?'display:none':''?>" id="responsavel_<?php echo$responsavelPossivel['rsuid']?>">
                    <tr>
                        <?php 
                            $i = 1;
                            if(is_array($cabecalho)){ 
                                foreach($cabecalho as $indices => $titulos){ 
                             echo '<th>'.$titulos.'</th>';
                        
                                }
                            } 
                        ?>
                    </tr>
                    <?php
                    if(is_array($dados)){
                        
                        $stBotaoExcluir           = '<img src="/imagens/excluir_01.gif" style="cursor:pointer;" title="Não é possível excluir esse Acompanhamento" border="0" onclick=" alert(\'Não é possível excluir essa Acompanhamento\')"> ';
                        $stBotaoExcluirHabilitado = '<img src="/imagens/excluir.gif" 	style="cursor:pointer;" title="Excluir"                                    border="0" onclick="javascript:ExcluirVistoria(\'' . $caminho_atual . 'acao=A\', %s );">';

                        $inRegistros = count( $dados );

                        if( count($dados) < 1 ){
                                echo "<tr><td colspan='10' style='text-align:center'>Nenhum registro encontrado.</td></tr>";
                        }

                        $msgObs = false;
                        $data = getUltimaEdicaoCronograma($obrid);
                        foreach($dados as $chave){

                            $obs = '';
//                            ver($data, $chave['supdtinclusao']);
                            if($data !== false){
                                if(strtotime($chave['supdtinclusao']) < strtotime($data)){

                                    if(verificaValorCronogramaSupervisao($obrid, $chave['supid'])) {
                                        $obs = '<span style="color: red">*</span>';
                                        $msgObs = true;
                                    }
                                }
                            }


                            // Não informado
                            if( $chave["realizacaosupervisao_rsuid"] != $responsavelPossivel['rsuid'] ) 
                                continue;
                            ?>

                            <tr <?php if($i%2) print("bgcolor=#f0f0f0") ?>>
                                <td align="center">
                                    <?php 
                                    
                                    //RN - Não permitir a edição/exclusão de vistorias pelo supervisor unidade ou gestor unidade 7 dias após a data de inclusão.
                                    $dataInicial  = explode('/',$chave["dtinclusao"]);
                                    $dataInicial  = $dataInicial[2].'-'.$dataInicial[1].'-'.$dataInicial[0];
                                    $dataFinal    = date('Y-m-d');
                                    $diffDatas    = ((((strtotime($dataFinal) - strtotime($dataInicial))/60)/60)/24);

                                    $validado     = (strtolower($chave["validadapelosupervisorunidade"]) == 's') ? true : false;

                                    $habilitaMods = ( ($diffDatas >= 7) && (possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE))  && ($validado == true) ) ? false : true;

                                    if ( $_GET['acao'] != 'V' && $habilitaMods === true ){
                                        
                                        echo '<img src="/imagens/alterar.gif" border="0" title="Editar" style="cursor:pointer;" onclick="javascript:AtualizarVistoria(\'?modulo=principal/inserir_vistoria&acao=A\', '.$chave["supid"].');">';

                                        
                                        /**
                                         * DEMANDA #229881
                                         */
                                        $perfil_cadastro = pegaPerfil($chave["usucpf"]);
                                        $perfil_sessao   = $_SESSION['usucpf'];
                                        $permissao_perfil_edicao = false;
                                        
                                        if($perfil_cadastro == PFLCOD_GESTOR_UNIDADE && $perfil_sessao == PFLCOD_SUPERVISOR_UNIDADE){
                                            $permissao_perfil_edicao = true;
                                        }
                                        
                                        if ( (($chave["usucpf"] == $_SESSION['usucpf'] || possui_perfil(array(PFLCOD_SUPER_USUARIO)) || $permissao_perfil_edicao ) && $inRegistros == $i) ){
                                            echo sprintf( $stBotaoExcluirHabilitado, $chave['supid'] );
                                        }else{
                                            echo $stBotaoExcluir;
                                        } 
                                    }else{
            				            echo '<img src="/imagens/alterar.gif" border="0" title="Visualizar vistoria" style="cursor:pointer;" onclick="javascript:AtualizarVistoria(\'?modulo=principal/inserir_vistoria&acao=V\', '.$chave["supid"].');">';
                                        echo $stBotaoExcluir;
                                    } 
                                     
                                    ?>          
                                                
                                </td>
                                <td align="center">
                                    <?php print($i); ?>
                                </td>
                                <td align="center">
                                    <?php print($chave["supid"]); ?>
                                </td>
                                <td align="center">
                                    <?php print($chave["dtvistoria"]); ?>
                                </td>
                                <td align="center">
                                    <?php print($chave["dtinclusao"]); ?>
                                </td>
                                <td>
                                    <?php if( possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_EMPRESA, PERFIL_ADMINISTRADOR) ) ){ ?>
                                        <img border="0" onclick='envia_email("<?php echo $chave["usucpf"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
                                    <?php } ?>
                                    <?php print($chave["usunome"]); ?>
                                </td>
                                <td>
                                    <?php print($chave["stadesc"]); ?>
                                </td>
                                <td>
                                    <?php if( !empty($chave["vistoriador"]) && ( possuiPerfil(array(PERFIL_SUPERVISORMEC, PERFIL_EMPRESA, PERFIL_ADMINISTRADOR) ) ) ){ ?>
                                        <img border="0" onclick='envia_email("<?php echo $chave["cpfvistoriador"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
                                    <?php } ?>
                                    <?php !empty($chave["vistoriador"]) ? print($chave["vistoriador"]) : print('NÃO INFORMADO'); ?>
                                </td>
                                <td>
                                    <?php print($chave["responsavel"]); ?>
                                </td>
                                <td align="center">
                                    <?php
                                        $percentual = $chave["percentual"];
                                        $percentual = $percentual > 100.00 ? 100.00 : $percentual;
                                        print(number_format($percentual,2,',','.'));
                                    ?> % <?=$obs?>
                                </td>
                                <?php if( possui_perfil(PFLCOD_GESTOR_UNIDADE) || possui_perfil(PFLCOD_SUPER_USUARIO) || possui_perfil(PFLCOD_SUPERVISOR_UNIDADE) || possui_perfil(PFLCOD_GESTOR_MEC) ): ?>
                                    <td align="center"><?php echo$chave['validado']?></td>
                                <?php endif; ?>
                            </tr>
                        <?php $i++; 
                        }
                    } ?>
                    <? if($msgObs): ?>
                    <tr>
                        <td colspan="11"> <span style="color: red"> *  Supervisão afetada pela alteração do cronograma. </span></td>
                    </tr>
                    <? endif;?>
                </table>
                </form><?php 

            }
            else if( $responsavelPossivel['rsuid'] == 2 ){ // MEC (FNDE) ############################################################################

                $supervisaoFnde = new SupervisaoFNDE();
                $param          = array();
                $param['obrid'] = $obrid;
                $sql = $supervisaoFnde->listaSql( $param );
                $cabecalho = array( "Ação",
                                    "Situação da Supervisão",
                                    "Data da Supervisão",
                                    "Data de Inclusão",
                                    "Responsável",
                                    "Cargo do Responsável",
                                    "Inserido Por",
                                    "Unidade em Funcionamento?");
                $dados = $db->carregar( $sql ); ?>

                <form id="formSupervisaoFnde" action="" method="post">
                    <input type="hidden" name="operacao" id="operacao" />
                    <input type="hidden" name="sfndeid" id="sfndeid" />
                </form>
                <script>
                    function alterarSupFNDE( sfndeid ){
                        $('#operacao').val('alterarSupervisaoFNDE');
                        $('#sfndeid').val(sfndeid);
                        $('#formSupervisaoFnde').submit();
                    }

                    function excluirSupFNDE( sfndeid ){
                        if ( confirm('Deseja apagar esta supervisão?') ){
                            $('#operacao').val('apagarSupervisaoFNDE');
                            $('#sfndeid').val(sfndeid);
                            $('#formSupervisaoFnde').submit();  
                        }
                    }
                </script>
                <table class="tabela" align="center" style="width: 98%;display:none" id="responsavel_<?php echo$responsavelPossivel['rsuid']?>">
                    <tr>
                    <?php foreach($cabecalho as $titulos){ ?>
                            <th><?php echo  $titulos  ?></th>
                    <?php } ?>
                    </tr>

                    <?php
                    $totReg = count( $dados );
                    $i      = 1;

                    if( !$dados[0]['acao'] ){
                        echo "<tr><td colspan='10' style='text-align:center'>Nenhum registro encontrado.</td></tr>";
                    }

                    if( $dados[0]['acao'] ){
                    foreach($dados as $chave){ ?>

                        <tr <?php if( $i%2 ) print("bgcolor=#f0f0f0") ?>>
                            <td align="center"><?php echo $chave['acao'] ?></td>
                            <td align="center"><?php echo $chave['esddsc'] ?></td>
                            <td align="center"><?php echo $chave['sfndedtsupervisao'] ?></td>
                            <td align="center"><?php echo $chave['sfndedtcadastro'] ?></td>
                            <td align="center"><?php echo $chave['entnome'] ?></td>
                            <td align="center"><?php echo $chave['sfndecargorepresentante'] ?></td>
                            <td align="center"><?php echo $chave['usunome'] ?></td>
                            <td align="center"><?php echo $chave['sfndefuncionamento'] ?></td>
                        </tr>
                        <?php $i++; 

                        }
                    }?>
                </table> <?php

            }
            else if( $responsavelPossivel['rsuid'] == 3 ){ // Empresa ############################################################################

                $param             = array();
                $param['obrid']    = $obrid;
                $supervisaoEmpresa = new SupervisaoEmpresa();
                $dados             = $supervisaoEmpresa->listaDados($param);
                ?>

                <form name="formularioEmpresa" id="formularioEmpresa" method="post" action="">
                    <input type="hidden" name="sueid"    id="sueid" value=""/>
                    <input type="hidden" name="operacao" id="operacao" value=""/>
                </form> 
                <script>
                    function imprimirLaudo( sueid ){
                        return windowOpen( '?modulo=principal/popupImpressaoLaudo&acao=A&sueid=' + sueid,'blank',
                                            'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
                    }
                    function excluirSupervisao( sueid ){
                        if(confirm("Deseja realmente excluir essa supervisão?")){
                            $('#sueid').val( sueid );
                            $('#operacao').val( 'apagarSupervisaoEmpresa' );
                            $('#formularioEmpresa').submit();
                        }
                    }
                    function editarSupervisao( sueid ){
                        $('#formularioEmpresa > #sueid').val( sueid );
                        $('#formularioEmpresa > #operacao').val('editarSupervisaoEmpresa');
                        $('#formularioEmpresa').submit();
                    }
                </script>
                <table class="tabela" align="center" style="width: 98%;" id="responsavel_<?php echo$responsavelPossivel['rsuid']?>">
                    <tr>
                    <?php 
                        $cabecalho = array( "Ação",
                                            "Nº OS",
                                            "Situação da Supervisão",
                                            "Data da Supervisão",
                                            "Data de Inclusão",
                                            "Responsável",
                                            "Cargo do Responsável",
                                            "Inserido Por",
                                            "% do Acompanhamento",
                                            "Unidade em Funcionamento?");
                        
                        foreach($cabecalho as $titulos){ ?>
                            <th><?php echo  $titulos  ?></th>
                        <?php } ?>
                    </tr>

                    <?php
                    $btnExc    = '<img src="/imagens/excluir_01.gif" border="0" style="cursor:pointer;" title="Não é possível excluir essa supervisão" onclick=" alert(\'Não é possível excluir essa supervisão\')"> ';
                    $btnExcHab = '<img src="/imagens/excluir.gif"    border="0" style="cursor:pointer;" title="Excluir"                                onclick="javascript:excluirSupervisao( %s );">';
                    $btnEdt    = '<img src="/imagens/alterar_pb.gif" border="0" style="cursor:pointer;" title="Não é possível editar essa supervisão"  onclick=" alert(\'Não é possível editar essa supervisão\')"> ';
                    $btnEdtHab = '<img src="/imagens/alterar.gif"    border="0" style="cursor:pointer;" title="Editar"                                 onclick="javascript:editarSupervisao( %s );">';
                    $btnPrtHab = '<img src="/imagens/print.png"      border="0" style="cursor:pointer;" title="Imprimir"                               onclick="javascript:imprimirLaudo( %s );"> ';

                    $btnPrintQuestionario = '<img style="cursor: pointer;" title="Imprimir questionário respondido" onclick="imprimirQuestionarioRespondido(%s, %s);" src="../imagens/print.png"/>';
                    
                    $totReg = count( $dados );
                    $i      = 1;

                    if( count($dados) < 1 ){
                        echo "<tr><td colspan='10' style='text-align:center'>Nenhum registro encontrado.</td></tr>";
                    }

                    foreach($dados as $chave){ ?>

                        <tr <?php if( $i%2 ) print("bgcolor=#f0f0f0") ?>>
                            <td align="center">
                            <?php
                            
                            $perfis_empresa = array( PFLCOD_EMPRESA_CONTRATADA, 
                                                     PFLCOD_EMPRESA_VISTORIADORA_GESTOR, 
                                                     PFLCOD_EMPRESA_VISTORIADORA_FISCAL);
                            if($chave['esdid'] != WF_ESDID_LAUDO_SUPERVISAO_CANCELADO){
                                if ($chave['esdid'] == WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO && !possuiPerfil(PFLCOD_SUPER_USUARIO) && possuiPerfil($perfis_empresa)){
                                    echo $btnEdt . "&nbsp;";
                                    echo $btnPrtHab;
    //                                echo sprintf( $btnPrtHab, $chave['sueid'] );
                                }else{
                                    if(!possui_perfil(array(PFLCOD_GESTOR_UNIDADE, PFLCOD_SUPERVISOR_UNIDADE, PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_EMPRESA_MI_FISCAL))){

                                        if ( $totReg != $i || possui_perfil(array(PFLCOD_CALL_CENTER)) ){
                                            echo $btnEdt . "&nbsp;";
                                            echo $btnExc;
                                        }else{
                                            echo sprintf($btnEdtHab . "&nbsp;", $chave['sueid']);
                                            echo sprintf($btnExcHab, $chave['sueid']);
                                        }
                                        if(!empty($chave['esdid'])){echo '&nbsp;'.sprintf( $btnPrtHab, $chave['sueid'] );};
                                    }

                                if ( ($chave['esdid'] == WF_ESDID_LAUDO_SUPERVISAO_HOMOLOGADO || $chave['esdid'] == WF_ESDID_LAUDO_SUPERVISAO_PAGAMENTO_SOLICITADO || $chave['esdid'] == WF_ESDID_LAUDO_SUPERVISAO_PAGO ) && is_numeric($chave['sueid']) && is_numeric($chave['sosid'])) {
                                        echo '&nbsp;'.sprintf( $btnPrintQuestionario, $chave['sueid'], $chave['sosid']);
                                    }
                                }
                            }
                            ?>
                            </td>
                            <td>
                                <?php print($chave["sosnum"]); ?>
                            </td>
                            <td>
                                <?php print($chave["esddsc"]); ?>
                            </td>
                            <td align="center">
                                <?php print($chave["suedtsupervisao"]); ?>
                            </td>
                            <td align="center">
                                <?php print($chave["suedtcadastro"]); ?>
                            </td>
                            <td>
                                <img border="0" onclick='envia_email("<?php echo $chave["entcpf"] ?>");' title="Enviar e-mail ao Responsável" src="../imagens/email.gif" style="cursor: pointer;"/>
                                <?php print($chave["entnome"]); ?>
                            </td>
                            <td>
                                <?php print($chave["suecargovistoriador"]); ?>
                            </td>
                            <td>
                                <img border="0" onclick='envia_email("<?php echo $chave["usucpf"] ?>");' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>
                                <?php print($chave["usunome"]); ?>
                            </td>
                            <td align="center">
                                <?php $percentual = $chave["percentual"];
                                $percentual = $percentual > 100.00 ? 100.00 : $percentual;
                                print(number_format($percentual,2,',','.')); ?> %
                            </td>
                            <td align="center">
                                <?php print($chave["suefuncionamento"]); ?>
                            </td>
                        </tr>
                        <?php $i++; 

                    } ?>
                </table> <?php

            } // else if( $responsavelPossivel['rsuid'] == 4 ){ // Não informado (Instituição que não foi informado) ###############################
            // }


        }

        monta_titulo('Empresa MI', '');
        ?>

            <table class="tabela" style="width: 98%;display:none" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="responsavel_4">
                <thead>
                <tr>
                    <th>Ação</th>
                    <th>Data da Evolução</th>
                    <th>Data de Inclusão</th>
                    <th>Responsável</th>
                    <!--  (Edificação) -->
                    <th>% Medido (Edificação)</th>
                    <th>% Validado (Edificação)</th>
                    <th>Valor Medido (R$) (Edificação)</th>
                    <th>Valor Validado (R$) (Edificação)</th>
                    <!--  (Serviços Externos) -->
                    <th>% Medido (Serviços Externos)</th>
                    <th>% Validado (Serviços Externos)</th>
                    <th>Valor Medido (R$) (Serviços Externos)</th>
                    <th>Valor Validado (R$) (Serviços Externos)</th>
                    <th>Possui Foto</th>
                    <th>Situação</th>
                    <th>Última tramitação</th>
                </tr>
                </thead>
                <tbody>
                <?php

                $param['where']['obrid'] = $obrid;
                $param['where']['emistatus'] = 'A';
                $evmi = new EvolucaoMi();
                $lista = $evmi->listaArray($param, true);

                if (empty($lista)) {
                    echo '<tr>
                      <td colspan="15" style="color:red; text-align: center"> Nenhuma evolução cadastrada. </td>
                  </tr>';
                } else {
                    $counter = 0;
                    $str_bg  = '';
                    foreach ($lista as $k => $v) {
                        $counter++;
                        $str_bg = ($counter % 2 == '') ? '' :' bgcolor="#FFFFFF" ';

                        $datae = explode('-', $v['emidtevolucao']);
                        $datae = $datae[2].'/'.$datae[1].'/'.$datae[0];

                        $datai = explode('-', $v['emidtinclusao']);
                        $datai = $datai[2].'/'.$datai[1].'/'.$datai[0];

                        echo '
                    <tr'.$str_bg.'>
                        <td style="width: 50px;text-align: center">'.$v['acao'].'</td>
                        <td style="text-align: center">'.$datae.'</td>
                        <td style="text-align: center">'.$datai.'</td>
                        <td style="text-align: center">'.$v['usunome'].'</td>
                        <!--  (Edificação) -->
                        <td style="text-align: center">'.number_format($v['emipecentmedidoedif'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emipecentvalidadoedif'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlmedidoedif'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlvalidadoedif'], 2, ',','.').'</td>
                        <!--  (Serviços Externos) -->
                        <td style="text-align: center">'.number_format($v['emipecentmedidoext'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emipecentvalidadoext'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlmedidoext'], 2, ',','.').'</td>
                        <td style="text-align: center">'.number_format($v['emivlvalidadoext'], 2, ',','.').'</td>
                        <td style="text-align: center">'.$v['possuifoto'].'</td>
                        <td style="text-align: center">'.$v['situacao'].'</td>
                        <td style="text-align: center">'.$v['ultima_tramitacao'].'</td>
                    </tr>
                  ';
                    }
                }
                ?>
                </tbody>
                <tfoot>
                <tr>
                    <th colspan="15">&nbsp;</th>
                </tr>
                <tr>
                    <td colspan="15"> &nbsp; </td>
                </tr>
                <tr>
                    <td colspan="14" style="text-align: center">
                        <input type="button" value="Cadastrar Nova Evolução" class="novaEvolucao">
                        <input type="button" value="Voltar para a Obra"     class="goObra">
                    </td>
                </tr>
                </tfoot>
            </table>
        <script type="text/javascript">
            $(document).ready(function(){

                editarEvolucaoMi = function(emiid){
                    window.location.href = 'obras2.php?modulo=principal/cadEvolucaoMi&acao=E&emiid='+emiid;
                }

                imprimirEvolucaoMi = function(emiid){
                    var url = 'obras2.php?modulo=principal/listaEvolucaoMi&acao=I&emiid='+emiid;
                    var popup = window.open(
                        url,
                        "Dados Evolução MI",
                        "width=1300,height=750,scrollbars=yes,scrolling=no,resizebled=no"
                    );
                }

            });
        </script>



        <form id="formSupervisaoMi" action="" method="post">
            <center>
                <input type="hidden" name="op" id="op">
                <input type="hidden" name="smiid" id="smiid">
            </center>
        </form>
        </td>
    </tr>
<?php

if( $habilitado ){
    echo '
    <tr bgcolor="#C0C0C0">
        <td>
            <div style="float: left;">';
                
                $obraContrato 	  = new ObrasContrato();
                $ocrvalorexecucao = $obraContrato->getValorContrato( $obrid );
                
                $itemComposicao = new ItensComposicaoObras();
                $somaIcoValor   = $itemComposicao->getSomaEtapaByObra( $obrid );

                // !possui_perfil(PFLCOD_GESTOR_UNIDADE)
                if ( ($ocrvalorexecucao == $somaIcoValor && $esdid != ESDID_OBJ_ADITIVO) || ($obra->tpoid == TPOID_MI_TIPO_B || $obra->tpoid == TPOID_MI_TIPO_C) ){
                    if( possuiPerfil(array(PFLCOD_EMPRESA_MI_FISCAL, PFLCOD_EMPRESA_MI_GESTOR, PFLCOD_CALL_CENTER)) && !possuiPerfil(array(PFLCOD_SUPER_USUARIO)) ){
                        echo '<input '.$desabilitaBotao.' style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Acompanhamento" onclick="window.location=\'?modulo=principal/cadEvolucaoMi&acao=A\'" />';
                    }else{

                        //$obrasArquivos 	= new ObrasArquivos();
                        $param = array("obrid" => $obrid/*, "not(img)" => true*/);

                        //valida tipo execução
                        $param['tpaid(IN)'] = '25';
                        $sql = $obrasArquivos->listaSqlVistoria( $param );

                        $valida_execucao = $db->carregar($sql);
                       //var_dump($valida_execucao);die();

                        //valida tipo fiscalização
                        $param['tpaid(IN)'] = '26';
                        $sql = $obrasArquivos->listaSqlVistoria( $param );

                        $valida_fiscalizacao = $db->carregar($sql);
                       //ver($valida_fiscalizacao,d);

                        if(!$valida_execucao || !$valida_fiscalizacao)
                        {
                            $desabilitaBotao ='disabled="disabled"';
                        }
                        echo '<input '.$desabilitaBotao.' style="cursor: pointer;" type="button" name="inserir_vistoria" value="Inserir Acompanhamento" onclick="window.location=\'?modulo=principal/inserir_vistoria&acao=A\'" />';

                        if(!$valida_execucao || !$valida_fiscalizacao)
                        {
                            echo "<span style='margin-left: 10px;font-weight: bold;color: #F00000'>Para inserir um acompanhamento é necessário inserir o ART de Execução e Fiscalização.</span>";
                        }





                    }
                }elseif ( $esdid == ESDID_OBJ_ADITIVO ){
                	echo '&nbsp;&nbsp;&nbsp;<font style="color: red;">Não é possivel inserir vistoria quando a obra está em celebração de aditivo.</font>';
                }else{
                	echo '&nbsp;&nbsp;&nbsp;<font style="color: red;">Valor do <b>cronograma</b> deve ser igual ao valor do <b>contrato</b> para liberar a <b>vistoria</b>.</font>';
                }
                
    echo '
            </div>
        </td>
    </tr>';
}elseif($str_trava_cronograma_vistoria != ''){ 
    echo '
    <tr bgcolor="#C0C0C0">
        <td>
            <div style="float: left;">
            &nbsp;&nbsp;&nbsp;
            <font style="color: red;">'.$str_trava_cronograma_vistoria.'</font>
            </div>
        </td>
    </tr>';
}else{ 
    echo '
    <tr bgcolor="#C0C0C0">
        <td>
            <div style="float: left;">
            &nbsp;&nbsp;&nbsp;
            <font style="color: red;">Só é possível inserir uma vistoria quando a obra estiver em execução, paralisada, inacabada ou concluída.</font>
            </div>
        </td>
    </tr>';
} ?>
</table>


<script type="text/javascript">
                    
    function ExcluirVistoria(url, supid){
        if(confirm("Deseja realmente excluir essa vistoria?")){
            window.location = url + '&subacao=VerificaVistoria&supid=' + supid;
        }
    }

    function AtualizarVistoria(url,supid){
        window.location = url + '&supid='+supid;
    }

    function atualizar(){
        document.getElementById('requisicao').value = 'atualizarRegistros';
        document.getElementById('formulario').submit();
    }

    function toggleParaObraVinculada(obj_tabela){
        var linha = ' <tr id="tr_obra_vinculada"> ' +
                    '    <td class="SubTituloDireita" width="20%">Percentual da última vistoria da última obra vinculada antes da atual:</td> ' +
                    '    <td> ' +
                    '        <span style="margin-left:5px"> ' +
                    '            <?php echo number_format($dados_obra_vinculada['obrpercentultvistoria_vinculado'],2,',','.').'%';?> ' +
                    '        </span> ' + 
                    '    </td> ' +
                    '</tr>';
        if( jQuery( '#chamada_obra_vinculada' ).is(':checked') ){
            jQuery(obj_tabela).append( linha );
        }else{
            jQuery('#tr_obra_vinculada').remove();
        }
    }

    function toggleParaResponsabilidades( id_tabela ){
        if( jQuery( '#chamada_'+id_tabela ).is(':checked') ){
            jQuery('#'+id_tabela).css('display','');
        }else{
            jQuery('#'+id_tabela).css('display','none');
        }
    }
    
    function imprimirQuestionarioRespondido(sueid, sosid){
	return windowOpen (
            '?modulo=principal/cadVistoriaEmpresaImpressaoPreenchido&acao=A&sueid='+sueid+'&sosid='+sosid,'blank'
          , 'height=700,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'
        );
    }
    
    function alterarSupMI (smiid) {
	$('#op').val('alterar');
	$('#smiid').val(smiid);
	$('#formSupervisaoMi').submit();
    }

    function excluirSupMI(smiid) {
        if (confirm('Deseja apagar esta supervisão?')) {
            $('#op').val('apagar');
            $('#smiid').val(smiid);
            $('#formSupervisaoMi').submit();
        }
    }
    
    function criaCopiaSupEmpresaMI(smiid){
        var url = 'obras2.php?modulo=principal/inserir_vistoria&acao=A&smiid='+smiid;
        window.location = url;
    }
    
    function abreTelaImpressao(smiid) {
        return windowOpen(
            '?modulo=principal/impressaoSupervisaoMI&acao=A&smiid='+smiid,'blank'
          , 'height=700,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'
        );
    }
    
</script>

<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(function (){
                        var forms = ['formulario_art','formulario','formSupervisaoFnde','formularioEmpresa','formSupervisaoMi'];
                        for(i=0; i<forms.length; i++ ){
                            bloqueiaForm(forms[0]);
                        }
                   }, 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).attr('disabled','disabled');
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('[name*=\"responsavel_\"]').find('img').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                   }
                 ";
            echo '</script>';
        }
?>

<?php //chkSituacaoObra(); ?>
