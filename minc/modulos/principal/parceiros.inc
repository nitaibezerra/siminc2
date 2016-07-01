<?php
if ($_REQUEST['requisicao'] == 'downloadArquivo'){
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    $file = new FilesSimec();
    $file->getDownloadArquivo($_REQUEST['arqid']);
    die();
}

?>
<script type="text/javascript" src="../includes/funcoes.js"></script>                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<style>
.fixIt{
	width: 20%;
}
</style>
<?php

if($_REQUEST['requisicao'] == 'removerVinculo'){
	
	$arParams = explode('_', $_REQUEST['params']);
	
	$sql = '';	
	if($arParams[0]){
		$sql .= "update minc.mceparceiro set entid = null, parhistoricoatuacao = null where parid = {$arParams[0]} and parstatus = 'A';";
	}
	if($arParams[1]){
		$sql .= "update minc.mceanexo set anestatus = 'I' WHERE arqid = {$arParams[1]} AND parid = {$arParams[0]}";
	}
	if($sql){
		$db->executar($sql);
		if($db->commit()){
			alert($sql);
			$db->sucesso('principal/parceiros', '');
		}
	}
	echo "
		<script>
			alert('Falha na operação, tente novamente!');
			document.location.href = 'minc.php?modulo=principal/parceiros&acao=A';
		</script>
		";
	die;	
}

if($_REQUEST['requisicao'] == 'deletaArquivo'){	
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$sql = "SELECT arqid FROM minc.mceanexo WHERE parid = {$_REQUEST['parid']}";
	$rsArq = $db->carregar($sql);	
	$rsArq = $rsArq ? $rsArq : array();
	$sqlDel = '';
	$arArqid = array();
	foreach($rsArq as $arq){
		$arArqid[] = $arq['arqid'];
		$sqlDel .= "DELETE FROM minc.mceanexo WHERE arqid = {$arq['arqid']};";
		$sqlDel .= "DELETE FROM public.arquivo WHERE arqid = {$arq['arqid']};";
	}
	if($sqlDel){
		$db->executar($sqlDel);
		if($db->commit()){
			foreach($arArqid as $arqid){
				$file->excluiArquivoFisico($arqid);		
			}
		}
	}
	$db->sucesso('principal/parceiros');		
	die();
}


if($_REQUEST['requisicao'] == 'recuperaNomeMunicipio'){
	$_REQUEST['muncod'] = intval($_REQUEST['muncod']);
	$sql = "SELECT mundescricao FROM territorios.municipio WHERE muncod = '{$_REQUEST['muncod']}'";
	$rs = $db->pegaUm($sql);
	echo $rs;
	die(); 
}

if(!empty($_SESSION['minc']['mceid'])){
	$sql = "SELECT parid, ent.entid, entnumcpfcnpj
			FROM minc.mceparceiro mce
			LEFT JOIN entidade.entidade ent ON ent.entid = mce.entid 
			WHERE parstatus='A'
			and mceid = {$_SESSION['minc']['mceid']}";
	$rs = $db->pegaLinha($sql);

	if(!empty($rs)){	
		if(!empty($rs['parid'])){
			
			$_REQUEST['parid'] = $rs['parid'];
		}
		
		if(empty($_REQUEST['campo_tipo_entidade'])){
			
			if(strlen($rs['entnumcpfcnpj']) > 11 ){	
				$_REQUEST['entid'] = $rs['entid'];
				$_REQUEST['campo_tipo_entidade'] = 2;	
			} 
			
			if(strlen($rs['entnumcpfcnpj']) == 11){	
				
				$_REQUEST['entid'] = $rs['entid'];
				$_REQUEST['campo_tipo_entidade'] = 1;	
			} 	
		}
			
		if(!empty($_REQUEST['campo_tipo_entidade'])){
			if($_REQUEST['campo_tipo_entidade'] == 1 && strlen($rs['entnumcpfcnpj']) == 11){
				$_REQUEST['entid'] = $rs['entid'];	
			}
			if($_REQUEST['campo_tipo_entidade'] == 2 && strlen($rs['entnumcpfcnpj']) > 11){
				$_REQUEST['entid'] = $rs['entid'];
			}
		}

        $_SESSION['avaliacao']['entidade'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $rs['entid'];

		//variavel para comparação de cpfcnpj na hora de salvar
		$cpfcnpjAnterior = $rs['entnumcpfcnpj'];
	} 
}

include_once APPRAIZ . 'includes/workflow.php';
require_once APPRAIZ . 'includes/classes/entidades.class.inc';
	
$boExisteEntidade = boExisteEntidade( $_SESSION['minc']['entid'] );
if( !$boExisteEntidade ){
	echo "<script>
			alert('A Entidade informada não existe!');
			history.back(-1);
		  </script>";
	die;
}

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

if(!$_SESSION['minc']['mceid']){
	echo "<script>
			alert('Não existe escola atribuída para o seu Perfil.');
			window.location.href = '/minc/minc.php?modulo=inicio&acao=C'; 
		  </script>";
	die;
}

$somenteConsulta = false;
$boSalvar = (!$somenteConsulta) ? true : false;
$campo_tipo_entidade = ($_REQUEST['campo_tipo_entidade']) ? $_REQUEST['campo_tipo_entidade'] : 1;

if($_GET['entid']){
	$sql = "SELECT entnumcpfcnpj FROM entidade.entidade WHERE entid = {$_GET['entid']}";
	$numcpfcnpj = $db->pegaUm($sql);
	if(strlen($numcpfcnpj) > 11){
		$campo_tipo_entidade = 2;
	}
}
$_SESSION['avaliacao']['tipo_entidade'] = $campo_tipo_entidade;

# Excluir Parceiros
if($_REQUEST['idParceiroExcluir']){
	//deleta associação do funid
	$sql = "DELETE FROM entidade.funentassoc WHERE feaid IN (SELECT fea.feaid FROM minc.mceparceiro mpa
														     LEFT JOIN entidade.funcaoentidade fen ON fen.entid = mpa.entid 
														     LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid   
														     WHERE mpa.parid='".$_REQUEST['idParceiroExcluir']."' AND fen.funid IN('".FUN_PARCEIRO_MC_PF."','".FUN_PARCEIRO_MC_PJ."'))";
	$db->executar($sql);
	//Deleta arquivo
	$arquivo = $db->pegaLinha("SELECT arqid, aneid FROM minc.mceanexo WHERE parid = {$_REQUEST['idParceiroExcluir']} AND anestatus = 'A'");
	if($arquivo['arqid']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$campos	= array("aneid" => $arquivo['aneid']);
		$file   = new FilesSimec("mceanexo", $campos, "minc");
		// Remove o registro do anexo antigo (pelo arqid) e o arquivo físico
		$file->setRemoveUpload( $arquivo['arqid'] );
	}	

	//Deleta parceiro
	//$sql = "DELETE FROM minc.mceparceiro WHERE parid = ". $_REQUEST['idParceiroExcluir'];
	$sql = "UPDATE minc.mceparceiro SET parstatus='I' WHERE parid= {$_REQUEST['idParceiroExcluir']}";
	
	$db->executar($sql);
	$db->commit();
	
	echo "<script>
				  alert('Parceiro excluido com sucesso.'); 
		          window.location.href = '/minc/minc.php?modulo=principal/parceiros&acao=A';
		  </script>";
	exit;
}

if ($_REQUEST['opt'] == 'salvarRegistro') {
	
	if(!$_REQUEST['funcoes']){
		echo '<script>
				  alert("Erro ao salvar, tente novamente. Obs: Use o Navegador Firefox!"); 
		          location.href = "minc.php?modulo=principal/parceiros&acao=A";
		  	</script>';
		exit;
	}
	
	$entidade = new Entidades();
	$entidade->carregarEntidade($_POST);
	$entidade->adicionarFuncoesEntidade($_REQUEST['funcoes']);
	$entidade->salvar();
	
	# Verifica se o campo quantidade alunos é numerico
	$parquantidadealuno = ($_POST['parquantidadealuno']) ? $_POST['parquantidadealuno'] : 0 ;
	if(!is_numeric($parquantidadealuno)){
		echo "<script>
				alert('Será aceito somente números no campo Quant. Alunos.');
				history.back(-1);
			  </script>";
		exit;
	}

	if(empty($_REQUEST['entidresp']) && $_REQUEST['entnumcpfcnpjresp']){
		$_REQUEST['entnumcpfcnpjresp'] = str_replace(array('.','/','-'), '', $_REQUEST['entnumcpfcnpjresp']);
		empty($_REQUEST['entdatanascresp']) ?  $_REQUEST['entdatanascresp'] = 'null' : $_REQUEST['entdatanascresp'] = "'".formata_data_sql($_REQUEST['entdatanascresp'])."'";
		$_REQUEST['endcepresp'] = str_replace(array('.','-'), '', $_REQUEST['endcepresp']);
		$_REQUEST['muncodresp'] = intval($_REQUEST['muncodresp']);

		$sql = "INSERT INTO entidade.entidade
					(entnumcpfcnpj, entsexo, entnome, entdatanasc, entnumdddresidencial, entnumresidencial,
					entnumrg, entorgaoexpedidor)
				VALUES
					('{$_REQUEST['entnumcpfcnpjresp']}', '{$_REQUEST['entsexoresp']}', '{$_REQUEST['entnomeresp']}',
					{$_REQUEST['entdatanascresp']}, '{$_REQUEST['entnumdddresidencialresp']}', '{$_REQUEST['entnumresidencialresp']}',
					'{$_REQUEST['entnumrgresp']}', '{$_REQUEST['entorgaoexpedidorresp']}'
					) returning entid;";

		$entidresp = $db->pegaUm($sql);

		if($entidresp && $_REQUEST['endcepresp']){
			$sql = "INSERT INTO entidade.endereco
						(endcep, endlog, endcom, endnum, endbai, estuf, muncod, entid)
					VALUES
						('{$_REQUEST['endcepresp']}', '{$_REQUEST['endlogresp']}', '{$_REQUEST['endcomresp']}',
						'{$_REQUEST['endnumresp']}', '{$_REQUEST['endbairesp']}', '{$_REQUEST['estufresp']}',
						'{$_REQUEST['muncodresp']}', {$entidresp});";

			$db->executar($sql);
		}
	}

	$entidresp = $entidresp ? $entidresp : ($_REQUEST['entidresp'] ? $_REQUEST['entidresp'] : 'null');
	$entid = $entidade->getEntid();

	//verificação de outra escola estar utilizando o mesmo parceiro. block!
	$sql = "SELECT distinct entid from minc.mceparceiro where parstatus = 'A' and entid = ".$entid." and mceid <> '".$_SESSION['minc']['mceid']."'";
	$boEntidade = $db->pegaUm( $sql );
	
	if( $boEntidade ){
		echo "<script>
				alert('A entidade selecionada como parceiro já está em parceria com outra escola. Favor utilizar outra entidade.');
				history.back();
			  </script>";
		exit();	
	}

	//verifica alteração de cpf ou cnpj, se sim, desativa o anterior e insere um novo parceiro.
	$cpfcnpjAnterior = str_replace(array(".", "/", "-"), "", $_REQUEST['cpfcnpjAnterior']);
	$cpfcnpjAtual = str_replace(array(".", "/", "-"), "", $_REQUEST['entnumcpfcnpj']);
	
	if($cpfcnpjAnterior != $cpfcnpjAtual && $_REQUEST['parid']){
		
		$sql = "UPDATE minc.mceparceiro SET	parstatus='I'
			 	WHERE parid = ".$_REQUEST['parid'];
		$db->executar($sql);
		
		unset($_REQUEST['parid']);
	}
	
	//insere ou altera um parceiro
	if(!$_REQUEST['parid']){
		$sql = "INSERT INTO minc.mceparceiro(mceid, entid, parquantidadealuno, parhistoricoatuacao, entidresplegal, parstatus)
    			VALUES (".$_SESSION['minc']['mceid'].", 
    					".$entid.",  
    					".$parquantidadealuno.",
    					'".substr($_REQUEST['parhistoricoatuacao'],0,1000)."', {$entidresp},'A') returning parid";
		$parid = $db->pegaUm($sql);
	} else {

		
		$sql = "UPDATE minc.mceparceiro
			   	SET
			   	entid={$entid}, 
			   	parquantidadealuno=".$parquantidadealuno.",
			   	parhistoricoatuacao = '".substr($_REQUEST['parhistoricoatuacao'],0,1000)."',
			   	paraceite = 'false',
			   	entidresplegal={$entidresp}
			 	WHERE parid = ".$_REQUEST['parid'];
		
		$db->executar($sql);
		$parid = $_REQUEST['parid'];
	}
	
	if(is_array($_REQUEST['chk_area'])){
		$sqlArea = '';
		if($_REQUEST['parid']){
			$sqlArea .= "DELETE FROM minc.mceparceiroatuacao WHERE parid = {$parid}; ";	
		}
		foreach($_REQUEST['chk_area'] as $area){
			$sqlArea .= "INSERT INTO minc.mceparceiroatuacao (araid, parid) VALUES ({$area}, {$parid})";
		}
		$db->executar($sqlArea);
	}
	
	//Anexa Arquivo
	if(!empty($_FILES['arquivo']['name'])){
		if($_FILES["arquivo"]["size"] > 3000000 || $_FILES["arquivo"]["type"] != "application/pdf"){
			echo "<script>
					alert('Permitidos apenas arquivos com extensão PDF e de tamanho até 3MB.');
					history.back();
					//window.location='minc.php?modulo=principal/parceiros&acao=A';
				  </script>";
			exit();	
		}	
		
		if($_FILES['arquivo']['error'] == 0){
			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
			$campos = array("parid"         => "'".$parid."'",
							"anetipo"		=> "1",
							"anedatahora" 	=> "NOW()",
							"anestatus"     => "'A'");
			$file = new FilesSimec("mceanexo", $campos ,"minc");
			$file->setUpload( NULL, $key = "arquivo" );		
		}
	}
	
	$db->commit();
	
	echo "<script>
			alert('Dados Gravados com sucesso');
			window.location='minc.php?modulo=principal/parceiros&acao=A';
		  </script>";
	exit();
}

require_once APPRAIZ . "includes/cabecalho.inc";
if($_REQUEST['campo_tipo_entidade'] == 2){
	require_once APPRAIZ . "www/includes/webservice/cpf.php";
}
echo '<br/>';

$sqlDocumento = "SELECT docid FROM minc.mcemaiscultura WHERE mceid = {$_SESSION['minc']['mceid']}";
$rsDocumento = !empty($_SESSION['minc']['mceid']) ? $db->pegaLinha($sqlDocumento) : array();

$docid = $rsDocumento['docid'];

$arMnuid = array();
if($docid){
    $sqlDocumento = "select * from workflow.documento where docid = {$docid}";
    $resultDocumento = $db->pegaLinha($sqlDocumento);

  	if ($resultDocumento && !in_array($resultDocumento['esdid'], array(/*ESTADO_DOCUMENTO_AVALIACAO, ESTADO_DOCUMENTO_FINALIZADO, ESTADO_DOCUMENTO_AVALIADO, */ESTADO_DOCUMENTO_ENVIADO_FNDE/*, ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR, ESTADO_DOCUMENTO_AVALIACAO*/))) {
        $arMnuid = array(MNUID_AVALIACAO, MNUID_MONITORAMENTO,MNUID_MONITORAMENTO2);
    }
}else{
	$arMnuid = array( MNUID_AVALIACAO, MNUID_MONITORAMENTO,MNUID_MONITORAMENTO2);
}

$db->cria_aba($abacod_tela, $url,'' , $arMnuid);

$titulo = "Mais Cultura nas Escolas";
$subtitulo = "Dados da Iniciativa Cultural Parceira";
echo monta_titulo($titulo, $subtitulo);

echo cabecalho();

$opcoes = array	("CPF" => array	("valor" => "1",
								 "id"    => "cpf",
								 "callback" => "tipoEntidade(this.value);"
					),
				 "CNPJ" => array ("valor"    => "2",
								  "id"    	 => "cnpj",
								  "callback" => "tipoEntidade(this.value);"	
					),
				);

if($_REQUEST['parid']){				
	$parid = $_REQUEST['parid'];
	$sql = "SELECT * FROM minc.mceparceiro WHERE parstatus='A' and parid = {$parid}";
	$rs = $db->pegaLinha($sql);
	if($rs) extract($rs);
	
	if($parhistoricoatuacao) {
		$parhistoricoatuacao = trim($parhistoricoatuacao);
		$parhistoricoatuacao = str_replace(chr(10),'',$parhistoricoatuacao);
		$parhistoricoatuacao = str_replace(chr(13),'',$parhistoricoatuacao);
		$parhistoricoatuacao = str_replace("'", '&lsquo;', $parhistoricoatuacao); 
	}
}



if($_SESSION['minc']['mceid']){
    
    // Validando inputs de acordo com o estado do documento
    $sqlDocumento = "SELECT docid
                                    FROM minc.mcemaiscultura 
                                    WHERE mceid = {$_SESSION['minc']['mceid']}";
    $rsDocumento = $db->pegaLinha($sqlDocumento);
    $docid = $rsDocumento['docid'];

    if($docid){
        $estado = wf_pegarEstadoAtual($docid);

         if(($estado['esdid'] != ESTADO_DOCUMENTO_CADASTRAMENTO) && ($estado['esdid'] != ESTADO_DOCUMENTO_CORRECAO_CADASTRADOR)){
            echo "<script>
                    setInterval(function(){
                            var inputs = document.getElementsByTagName('input');
                            var selects = document.getElementsByTagName('select');
                            var textareas = document.getElementsByTagName('textarea');

                            for(var i=0; i<textareas.length; i++){
                                textareas[i].disabled = true;
                             }

                            for(var i=0; i<selects.length; i++){
                                selects[i].disabled = true;
                             }

                            for(var i=0; i<inputs.length; i++){
                                inputs[i].disabled = true;
                             }

                            /* document.getElementById('btngravar').disabled = 1 */
                    },100);
                  </script>  
            ";
        }
    }
}

$perfis = arrayPerfil();

if($parid){
	$sql = "SELECT max(arqid)
			FROM minc.mceanexo
			WHERE parid = {$parid} AND anestatus = 'A'";
	$arqid = $db->pegaUm($sql);

	if($arqid){
		$sql = "SELECT *
				FROM public.arquivo
				WHERE arqid = {$arqid}";
		$arquivo = $db->pegaLinha($sql);
		$arquivo['arqnome'] = str_replace(array("'", '"'), '', $arquivo['arqnome']);
	}
}
	
?>	
<script>

	jQuery(function(){		
		jQuery('.removerVinculo').click(function(){
			if(confirm('Deseja remover o vínculo da escola?')){
				document.location.href = 'minc.php?modulo=principal/parceiros&acao=A&requisicao=removerVinculo&params='+this.id;
			}
		});
	});
</script>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tbody id="tableEntidade">
<tr>
<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Tipo de busca:</label></td>
<td>
	<?php //echo campo_radio( 'cpfcnpj', $opcoes, 'h', true ); ?>
	
	<input type="radio" name="cpfcnpj" id="cpf" value="1" onclick="tipoEntidade(this.value);"> CPF
	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
	<input type="radio" name="cpfcnpj" id="cnpj" value="2" onclick="tipoEntidade(this.value);"> CNPJ
</td>
</tr>

<?php if( ( in_array(PERFIL_MINC_ADMINISTRADOR, $perfis) || $db->testa_superuser() ) && $rs['entid']  ) : ?>
	<tr>
		<td class="SubtituloDireita">Remover vínculo</td>
		<td>		
		<input type="button" value="Remover" id="<?php echo $_REQUEST['parid'].'_'.$arquivo['arqid']; ?>" class="removerVinculo" />
		</td>
	</tr>
<?php endif; ?>
</tbody>
</table>

<table width="95%" align="center" cellSpacing="0" cellPadding="0">
	<tr>
		<td>
			<?php
                            $entidade = new Entidades();	
                            if($_REQUEST['entid']) {
                                    $entidade->carregarPorEntid($_REQUEST['entid']);
      
                            }
            //ver($_SESSION['minc']['entid'],$_REQUEST['entid']);
                            switch($campo_tipo_entidade) {
                                case 1:
                                    echo $entidade->formEntidade("minc.php?modulo=principal/parceiros&acao=A&opt=salvarRegistro",array("funid" => FUN_PARCEIRO_MC_PF, "entidassociado" => $_SESSION['minc']['entid']),
                                    array("enderecos"=>array(1),"obrigatorios"=>array("entnumcpfcnpj","entnome","entemail","entnumrg","entorgaoexpedidor","entsexo","entdatanasc","entnumresidencial","entnumcelular","endlog1","endnum1")));
                                    break;
                                case 2:
                                    echo $entidade->formEntidade("minc.php?modulo=principal/parceiros&acao=A&opt=salvarRegistro",array("funid" => FUN_PARCEIRO_MC_PJ, "entidassociado" => $_SESSION['minc']['entid']),
                                    array("enderecos"=>array(1),"obrigatorios"=>array("entnumcpfcnpj","entnome","entemail","entnumcomercial","njuid","tpcid", "entrazaosocial")));
                                    break;
                                default:
                                    die("Problemas com a função da entidade");
                            }
                            

                            if( (count( $perfis ) == 1 && in_array(PDEESC_PERFIL_CONSULTA_MAIS_EDUCACAO, $perfis)) || !$boSalvar ){
                                echo "
                                    <script>
                                        /* document.getElementById('btngravar').disabled = 1; */
                                    </script> 
                                ";				
                                $imgExcluir     = "/imagens/excluir_01.gif";			 
                                $onclickExcluir = "return false";			 
                            }else{
                                $imgExcluir     = "/imagens/excluir.gif";			 
                                $onclickExcluir = "excluirParceiro(' || mepar.parid || ');";	
                            }
			?>
		</td>
	</tr>
</table>



<script type="text/javascript">

jQuery(document).ready(function(){
        
        jQuery('#btngravar').attr('disabled', true);
        jQuery('#btncancelar').attr('disabled', true);
        jQuery('.imagemExcluir').attr('disabled', true);
        
        <?php if(in_array(PERFIL_MINC_CADASTRADOR, $perfis) || in_array(PERFIL_MINC_SUPER_USUARIO, $perfis)){ ?>
            jQuery('#btngravar').attr('disabled', false);
            jQuery('#btncancelar').attr('disabled', false);
            jQuery('.imagemExcluir').attr('disabled', false);
        <?php } ?>
            
});

//jQuery.noConflict();
jQuery('#tr_funid').hide();

//jQuery('#endcep1').parent().append('<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
jQuery('#endlog1').parent().append('<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');
jQuery('#endnum1').parent().append('<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">');


jQuery(function(){
	jQuery('.deletaArquivo').click(function(){
		if(confirm('Deseja deletar o arquivo?')){
			document.location.href = this.id;
		}
	});
	
});

jQuery('#btncancelar').val('Limpar');
jQuery('#btncancelar').click( function(){
var $inputs = jQuery('#frmEntidade :input');
$inputs.each( function(){
	if( this.type != 'button' && this.type != 'submit' )
		jQuery(this).val('');
	});
});

jQuery('#tr_entnumdddresidencial').find("[class='SubTituloDireita']").html("Telefones (Residencial, Comercial, Celular):");
jQuery('#tr_entnumdddcomercial').find("[class='SubTituloDireita']").html("");
jQuery('#tr_entnumdddcelular').find("[class='SubTituloDireita']").html("");
var $tr = jQuery('#tr_entnumdddfax');
var $next = $tr.next();
$tr.clone(true).insertAfter($next[0]);
     $tr.remove();

var tipo_entidade = <?php echo $campo_tipo_entidade; ?>;

if(tipo_entidade == 2){
	jQuery('#cnpj').attr('checked', true);
} else {
	jQuery('#cpf').attr('checked', true);
}

var arquivo = '';

<?php if($arqid): ?>
arquivo = '<a href="javascript:void(0)" id="minc.php?modulo=principal/parceiros&acao=A&requisicao=deletaArquivo&arqid=<?php echo $arquivo['arqid']; ?>&parid=<?php echo $parid; ?>" class="deletaArquivo"><img border="0" class="imagemExcluir" "src="../imagens/exclui_p.gif" /></a>&nbsp;<a href="minc.php?modulo=principal/parceiros&acao=A&requisicao=downloadArquivo&arqid=<?php echo $arquivo['arqid']; ?>"><?php echo $arquivo['arqnome'].'.'.$arquivo['arqextensao']; ?></a>';
<?php else: ?>
arquivo = "<input type='file' name='arquivo' id='arquivo'>&nbsp;<img border='0' title='Indica campo obrigatório.' src='../imagens/obrig.gif'>";
<?php endif; ?>

historico = '<textarea onkeyup="textCounter(this.form.parhistoricoatuacao,this.form.no_parhistoricoatuacao,1000);" onkeydown="textCounter(this.form.parhistoricoatuacao,this.form.no_parhistoricoatuacao,1000);" style="width:45 ex;" onblur="MouseBlur(this);textCounter(this.form.parhistoricoatuacao,this.form.no_parhistoricoatuacao,1000);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);" onmouseover="MouseOver(this);" rows="5" cols="45" name="parhistoricoatuacao" id="parhistoricoatuacao" class=""><?php echo $parhistoricoatuacao ? $parhistoricoatuacao : '' ?></textarea><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"><br/><input type="text" class="CampoEstilo" value="1000" maxlength="6" size="6" name="no_parhistoricoatuacao" style="text-align:right;border-left:#888888 3px solid;color:#808080;" readonly="">';

jQuery('#tr_endmapa').after('<tr id="tr_historico"><td class="SubTituloDireita" width="30%"><strong>Histórico de atuação </strong><br /><br />Detalhar experiências anteriores da Iniciativa Cultural Parceira, relacionadas ao Plano de Atividade Cultural da Escola proposto e aos eixos temáticos escolhidos. Nesse campo podem ser ofercidos endereços eletrônicos por meio dos quais seja possível tomar conhecimento de registros e/ou trabalhos atuais  da Iniciativa Cultural Parceira.</td><td>'+historico+'</td></tr>');
jQuery('#tr_historico').after('<tr id="tr_anexo"><td class="SubTituloDireita"><strong>Anexar Portfólio</strong><br /><br />Portfólios são arquivos que, por meio de textos e imagens, expõem os trabalhos desenvolvidos pelo artista/ iniciativa cultural parceira de maneira objetiva. Podem ser anexados arquivos em formato PDF de até 3MB.</td><td>'+arquivo+'</td></tr>');
jQuery('#frmEntidade').attr('encoding','multipart/form-data');	
document.getElementById('tblentidade').style.width = '100%';

<?
/*
 * Quando for pessoa juridica filtrar alguns campos do formulario
 */
switch($campo_tipo_entidade) {
	case 1:
?>
	/*
	 * DESABILITANDO O NOME DA ENTIDADE
	 */
	 
	document.getElementById('entnome').readOnly = true;
	document.getElementById('entnome').className = 'disabled';
	document.getElementById('entnome').onfocus = "";
	document.getElementById('entnome').onmouseout = "";
	document.getElementById('entnome').onblur = "";
	document.getElementById('entnome').onkeyup = "";

	function enviaForm(){
		if(jQuery('#entnumcpfcnpj').val() == ''){
			alert('O CPF é obrigatório.');
	    	return false;	
		}
		if(!validar_cpf(jQuery('#entnumcpfcnpj').val())){
			alert('O CPF é inválido.');
	    	return false;	
		}
		if( jQuery('#entrazaosocial').val() == '' ){
			alert('A Razão Social é o obrigatória');
	    	return false;	
		}
		if(jQuery('#entnome').val() == ''){
			alert('O nome é obrigatório.');
	    	return false;	
		}
		if(jQuery('#entemail').val() == ''){
			alert('O e-mail é obrigatório.');
	    	return false;	
		}else{
			var x=jQuery('#entemail').val();
			var atpos=x.indexOf("@");
			var dotpos=x.lastIndexOf(".");
			if (atpos<1 || dotpos<atpos+2 || dotpos+2>=x.length)
			{
			  alert("Favor inserir um email válido.");
			  return false;
			}
		}
		
		if(jQuery('#entnumrg').val() == ''){
			alert('O RG é obrigatório.');
	    	return false;	
		}
		if(jQuery('#entorgaoexpedidor').val() == ''){
			alert('O órgão expedidor é obrigatório.');
	    	return false;	
		}
		if(jQuery('#entsexo').val() == ''){
			alert('O sexo é obrigatório.');
	    	return false;	
		}
		if(jQuery('#entdatanasc').val() == ''){
			alert('A data de nascimento é obrigatório.');
			return false;
	 	} 
	    if(!validaData(document.getElementById('entdatanasc'))){
	    	alert('Data de nascimento é inválida.');
	    	return false;
	    } 	
		if(jQuery('#entnumdddresidencial').val() == '' || jQuery('#entnumresidencial').val() == ''){
			alert('O telefone residencial é obrigatório.');
	    	return false;
		}
		if(jQuery('#entnumdddcelular').val() == '' || jQuery('#entnumcelular').val() == ''){
			alert('O telefone celular é obrigatório.');
	    	return false;
		}		
		
		if(jQuery('#endlog1').val() == ''){
			alert('O Logradouro do Endereço é obrigatório.');
			return false;
		}
		if(jQuery('#endnum1').val() == ''){
			alert('O Número do Endereço é obrigatório.');
			return false;
		}
		
	    if(jQuery('#parhistoricoatuacao').val() == ''){
	    	alert('O histórico de atuação é obrigatório.');
	    	return false;
	    }else{
	    
			var hist = document.getElementById('parhistoricoatuacao');
			var tam=hist.value.split("");
			if (tam.length>999){			
				alert("Histórico de Atuação não deve ultrapassar 1000 caracteres");
				var newStr = hist.value.substring(0,(tam.length - 999));
				jQuery('#parhistoricoatuacao').val(newStr);
				return false;
			}
	    }
	    if(jQuery('#arquivo').val() == ''){
	    	alert('O portfólio é obrigatório.');
	    	return false;
	    } 			
		jQuery('#frmEntidade').submit();
	}
<?
	break;
	case 2:
?>
	document.getElementById('tr_entnuninsest').style.display = 'none';
	document.getElementById('tr_entungcod').style.display = 'none';
	document.getElementById('tr_tpctgid').style.display = 'none';
	document.getElementById('tr_entunicod').style.display = 'none';
	document.getElementById('tr_entcodent').style.display = 'none';
	/*
	 * DESABILITANDO O NOME DA ENTIDADE
	 */
	 
	jQuery('#entrazaosocial').removeClass('disabled')
	jQuery('#entrazaosocial').attr('readonly',false);	
	 
	document.getElementById('entnome').readOnly = true;
	document.getElementById('entnome').className = 'disabled';
	document.getElementById('entnome').onfocus = "";
	document.getElementById('entnome').onmouseout = "";
	document.getElementById('entnome').onblur = "";
	document.getElementById('entnome').onkeyup = "";

	function enviaForm(){
		if(jQuery('#entnumcpfcnpj').val() == ''){
			alert('O CNPJ é obrigatório.');
	    	return false;	
		}
		if(!validarCnpj(jQuery('#entnumcpfcnpj').val())){
			alert('O CNPJ é inválido.');
	    	return false;	
		}
		if(jQuery('#entnome').val() == ''){
			alert('O nome é obrigatório.');
	    	return false;	
		}
		if(jQuery('#entemail').val() == ''){
			alert('O e-mail é obrigatório.');
	    	return false;	
		}		
		if(jQuery('#entnumdddcomercial').val() == '' || jQuery('#entnumcomercial').val() == ''){
			alert('O telefone comercial é obrigatório.');
			return false;
		}			
		if(jQuery('#njuid').val() == ''){
			alert('A natureza jurídica é obrigatória.');
	    	return false;
		}	
		if(jQuery('#tpcid').val() == ''){
			alert('A classificação é obrigatória.');
	    	return false;
		}			
		if(jQuery('#entnumcpfcnpjresp').val() == ''){
			alert('O CPF do responsável é obrigatório.');
	    	return false;
		}	
		if(jQuery('#entnomeresp').val() == ''){
			alert('O nome do responsável é obrigatório.');
	    	return false;
		}			
		
		if(jQuery('#endlog1').val() == ''){
			alert('O Logradouro do Endereço é obrigatório.');
			return false;
		}
		if(jQuery('#endnum1').val() == ''){
			alert('O Número do Endereço é obrigatório.');
			return false;
		}
		
	    if(jQuery('#parhistoricoatuacao').val() == ''){
	    	alert('O histórico de atuação é obrigatório.');
	    	return false;
	    } 			
	    if(jQuery('#arquivo').val() == ''){
	    	alert('O portfólio é obrigatório.');
	    	return false;
	    } 		
		jQuery('#frmEntidade').submit();
	}
<?
	break;
}
?>

function tipoEntidade(campo_tipo_entidade){
	window.location.href = "/minc/minc.php?modulo=principal/parceiros&acao=A&campo_tipo_entidade="+campo_tipo_entidade;
}
 
function excluirParceiro(idParceiro){
	if(confirm('Deseja Realmente excluir este Parceiro')){
		window.location.href = "/minc/minc.php?modulo=principal/parceiros&acao=A&idParceiroExcluir="+idParceiro;
		return true;
	} else {
		return false;
	}
}

function popupMapa(entid){
	//window.open('pdeescola.php?modulo=principal/mapaEntidade&acao=A&entid=' + entid,'Mapa','scrollbars=yes,height=700,width=840,status=no,toolbar=no,menubar=no,location=no');
	window.open('http://culturaeduca.cc');
}

</script>

<?php if($_REQUEST['parid']): ?>
	<script>
		jQuery('#tr_entnumcpfcnpj').find('input').after(' <input type="hidden" name="parid"> <input type="hidden" name="cpfcnpjAnterior"> ');
		jQuery('[name=parid]').val('<?php echo $_REQUEST['parid']; ?>');
		jQuery('[name=cpfcnpjAnterior]').val('<?php echo $cpfcnpjAnterior; ?>');
	</script>
	
<?php endif; ?>

<?php if($campo_tipo_entidade == 2): ?>
	<script>
		jQuery(function(){
			jQuery('#tr_tpsid').hide();
			jQuery('#tr_funcoescadastradas').hide();
			// Insere campos do Responsável Legal
			var campoID 	= '<input type="hidden" name="entidresp" id="entidresp" value="" />';
			var campoCPF 	= '<input type="text" onblur="MouseBlur(this);getEntidadePeloCPFparceiros(this.value);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onkeyup="this.value=mascaraglobal(\'###.###.###-##\',this.value);" class="normal" value="" maxlength="14" size="17" id="entnumcpfcnpjresp" name="entnumcpfcnpjresp"><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
			var campoRG 	= '<input type="hidden" name="entnumrgresp" id="entnumrgresp" readonly="" class="disabled" size="15" value="" >';
			var campoExp 	= '<input type="hidden" name="entorgaoexpedidorresp" id="entorgaoexpedidorresp" readonly="" class="disabled" size="15" value="" >';
			var campoNas 	= '<input type="hidden" name="entdatanascresp" id="entdatanascresp" readonly="" class="disabled" size="15" value="" >';
			//var campoDDD 	= '<input type="text" name="entnumdddresidencialresp" id="entnumdddresidencialresp" readonly="" class="disabled" size="3" value="" >';
			//var campoTEL 	= '<input type="text" name="entnumresidencialresp" id="entnumresidencialresp" readonly="" class="disabled" size="15" value="" >';
			var campoNOME 	= '<input type="text" name="entnomeresp" id="entnomeresp" readonly="" class="disabled" size="90" value=""><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
			//var campoCEP 	= '<input type="text" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onkeyup="this.value=mascaraglobal(\'#####-###\',this.value);" class="disabled" value="" maxlength="9" size="11" id="endcepresp" name="endcepresp">';
			//var campoSex 	= '<input type="radio" name="entsexoresp" id="entsexoresp_m" value="M" />&nbsp;Masculino&nbsp;<input type="radio" name="entsexoresp" id="entsexoresp_f" value="F" />&nbsp;Feminino&nbsp;';			
			//var campoLog 	= '<input type="text" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" class="disabled" value="" size="65" id="endlogresp" name="endlogresp">';
			//var campoCom 	= '<input type="text" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" class="disabled" value="" size="65" id="endcomresp" name="endcomresp">';
			//var campoNum 	= '<input type="text" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" class="disabled" value="" size="11" id="endnumresp" name="endnumresp">';
			//var campoBai 	= '<input type="text" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" class="disabled" value="" size="45" id="endbairesp" name="endbairesp">';
			//var campoUF 	= '<input type="text" name="estufresp" id="estufresp" readonly="" class="disabled" size="8" value="" >';
			//var campoMun 	= '<input type="hidden" name="muncodresp" id="muncodresp" readonly="" class="disabled" size="18" value="" >';
			//var campoMud 	= '<input type="text" name="mundescresp" id="mundescresp" readonly="" class="disabled" size="18" value="" >';

			var htmlResponsavel  = '<tr><td colspan="2" class="subtituloCentro">RESPONSÁVEL LEGAL</td></tr>';
				htmlResponsavel += '<tr><td class="subtituloDireita">CPF</td><td>'+campoCPF+campoID+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_sexo_resp" style="display:none;"><td class="subtituloDireita">Sexo</td><td>'+campoSex+'</td></tr>';				
				htmlResponsavel += '<tr id="tr_nome_resp"><td class="subtituloDireita">Nome</td><td>'+campoNOME+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_nasc_resp" style="display:none;"><td class="subtituloDireita">Nascimento</td><td>'+campoNas+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_fone_resp" style="display:none;"><td class="subtituloDireita">Telefone</td><td>'+'('+campoDDD+')&nbsp;'+campoTEL+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_rg_resp" style="display:none;"><td class="subtituloDireita">RG</td><td>'+campoRG+'&nbsp;Orgão Expedidor&nbsp;'+campoExp+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_end_resp" style="display:none;"><td colspan="2" class="subtituloCentro">ENDEREÇO - RESPONSÁVEL LEGAL</td></tr>';
				//htmlResponsavel += '<tr id="tr_cep_resp" style="display:none;"><td class="subtituloDireita">CEP</td><td>'+campoCEP+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_log_resp" style="display:none;"><td class="subtituloDireita">Logradouro</td><td>'+campoLog+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_com_resp" style="display:none;"><td class="subtituloDireita">Complemento</td><td>'+campoCom+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_num_resp" style="display:none;"><td class="subtituloDireita">Número</td><td>'+campoNum+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_bai_resp" style="display:none;"><td class="subtituloDireita">Bairro</td><td>'+campoBai+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_uf_resp" style="display:none;"><td class="subtituloDireita">UF</td><td>'+campoUF+'</td></tr>';
				//htmlResponsavel += '<tr id="tr_muncod_resp" style="display:none;"><td class="subtituloDireita">Município</td><td>'+campoMun+campoMud+'</td></tr>';
			jQuery('#tr_entobs').after(htmlResponsavel);
		});

		function getEntidadePeloCPFparceiros(cpf) {
			if(cpf) {
				if(!validar_cpf(cpf)) {
					alert('CPF inválido!');
					return false;		
				}
				displayStaticMessage("<p align=center>Carregando...</p>","");
				var myAjax = new Ajax.Request('/geral/consultadadosentidade.php?requisicao=pegarentidadePorcpfcnpj&entnumcpfcnpj=' + cpf + "&funidEspecifico=" + funidEspecifico,
					{
						method: 'post',
						asynchronous: false,
						onComplete: function(resp) {
							if(resp.responseText == "naoexiste") {
								getUsuarioReceitaPeloCPFparceiros(cpf);		
								closeMessage();
							} else if(resp.responseText.substr(0,12) == "existemuitos") {
								ProcessaMuitasEntidades(resp.responseText, 'fisica');
								return false;
							} else {
								var dados = resp.responseText.split("||");
								jQuery('#entidresp').val(dados[0]);
								jQuery('#entnomeresp').val(dados[3]);
								closeMessage();
							}
						}
					});
			}
		}

		function getUsuarioReceitaPeloCPFparceiros(cpf) {
			var comp = new dCPF();
			comp.buscarDados(cpf);	
			document.getElementById('entidresp').value = "";
			if(comp.dados.no_pessoa_rf){
				jQuery('#entnomeresp').val(comp.dados.no_pessoa_rf);
			}
			if(comp.dados.nu_rg){ 
				jQuery('#entnumrgresp').val(comp.dados.nu_rg);
				jQuery('#entorgaoexpedidorresp').val(comp.dados.ds_orgao_expedidor_rg);
				jQuery('#tr_rg_resp').show();
			}	
			if(comp.dados.sg_sexo_rf){
				 if(comp.dados.sg_sexo_rf == 'M'){
					 jQuery('#entsexoresp_m').attr('checked', true);
				 }else{
					 jQuery('#entsexoresp_f').attr('checked', true);
				 }
				 jQuery('#tr_sexo_resp').show();
			}
			if(comp.dados.dt_nascimento_rf){
				jQuery('#entdatanascresp').val(comp.dados.dt_nascimento_rf.substr(6,2)+'/'+comp.dados.dt_nascimento_rf.substr(4,2)+'/'+comp.dados.dt_nascimento_rf.substr(0,4));
				jQuery('#tr_nasc_resp').show();
			}			
			if(comp.dados.ds_contato_pessoa) {
				arFone = comp.dados.ds_contato_pessoa.split('-');
				jQuery('[name=entnumdddresidencialresp]').val(arFone[0]);
				jQuery('[name=entnumresidencialresp]').val(arFone[1]);
				jQuery('#tr_fone_resp').show();
			}

			if(comp.dados.nu_cep) {
				jQuery('#endcepresp').val(comp.dados.nu_cep);
				jQuery('#tr_cep_resp').show();
			}

			if(comp.dados.ds_logradouro) {
				jQuery('#endlogresp').val(comp.dados.ds_logradouro);
				jQuery('#tr_log_resp').show();
			}

			if(comp.dados.ds_logradouro_comp) {
				jQuery('#endcomresp').val(comp.dados.ds_logradouro_comp);
				jQuery('#tr_com_resp').show();
			}

			if(comp.dados.ds_bairro) {
				jQuery('#endbairesp').val(comp.dados.ds_bairro);
				jQuery('#tr_bai_resp').show();
			}

			if(comp.dados.ds_bairro) {
				jQuery('#endbairesp').val(comp.dados.ds_bairro);
				jQuery('#tr_bai_resp').show();
			}

			if(comp.dados.ds_numero) {
				jQuery('#endnumresp').val(comp.dados.ds_numero);
				jQuery('#tr_num_resp').show();
			}
			
			if(comp.dados.sg_uf) {
				jQuery('#estufresp').val(comp.dados.sg_uf);
				jQuery('#tr_uf_resp').show();
			}
			
			if(comp.dados.co_cidade) {
				jQuery.ajax({
					url		: 'minc.php?modulo=principal/parceiros&acao=A',
					type	: 'post',
					data	: 'requisicao=recuperaNomeMunicipio&muncod='+comp.dados.co_cidade,
					success	: function(e){
						jQuery('#mundescresp').val(e);
					}					
				});
				
				jQuery('#muncodresp').val(comp.dados.co_cidade);
				jQuery('#tr_muncod_resp').show();
			}

		}
		
	</script>
	
	<?php 
	if($_REQUEST['parid']){ ?>
		<?php 
		$sql = "SELECT ent.entid, ent.entnumcpfcnpj, ent.entnome 
				FROM minc.mceparceiro par
				LEFT JOIN entidade.entidade ent on ent.entid = par.entidresplegal
				WHERE parstatus='A' and parid = {$_REQUEST['parid']}";
		$rsRepresentante = $db->pegaLinha($sql); ?>
		<?php if($rsRepresentante){ ?>
			<?php $usucpfrep = formatar_cpf($rsRepresentante['entnumcpfcnpj']); ?>
			<script>
				jQuery(function(){
					jQuery('#entnumcpfcnpjresp').val('<?php echo $usucpfrep; ?>');
					jQuery('#entnomeresp').val('<?php echo $rsRepresentante['entnome'] ?>');
					jQuery('#entidresp').val('<?php echo $rsRepresentante['entid'] ?>');
				});
			</script>
		<?php } } ?>
<?php endif; ?>

<?php if(verificaEncerramentoPlanoEscola()): ?>

	<script>
		jQuery(function(){			
			jQuery.each(jQuery('#frmEntidade')[0].elements, function(i, v){
				jQuery(v).attr('disabled', true).removeClass('normal').addClass('disabled');
			});
			jQuery('#btncancelar, #btngravar, #tr_acoes').remove();
		});
	</script>

<?php endif; ?>