<?php
monta_titulo('','Dados da Transferência');
$_SESSION['projovemurbano']['modalidade'] = '';
$bloq = 'S'
?>
<?
// $sql = "SELECT 
// 			perdtfim
// 		FROM 
// 			projovemurbano.periodocurso
// 		WHERE
// 			perid = (SELECT DISTINCT
// 		      MAX (per.perid)
// 		 FROM
// 		    projovemurbano.diario dia
// 		 INNER JOIN projovemurbano.periodocurso per ON per.perid = dia.perid
// 		 WHERE
// 			to_char(perdtfim,'YYYYMMDD') <= to_char(now(),'YYYYMMDD'))";

// $dataUltimoPeriodo = $db->pegaUm($sql);
// $aposSeisDias      = somar_dias_uteis( $dataUltimoPeriodo, 6 );
// $aposOitoDias	   = somar_dias_uteis( $dataUltimoPeriodo, 8 );

//     $sql="SELECT 
//             true
//          FROM 
//             projovemurbano.periodocurso
//          WHERE 
//             (to_char($aposSeisDias,'YYYYMMDD') <= to_char(now(),'YYYYMMDD'))           
//          AND (to_char($aposOitoDias,'YYYYMMDD') >= to_char(now(),'YYYYMMDD'))";

//    $permiteTransferencia = $db->pegaUm($sql);  
   
   $sql2 = "SELECT
                true
            FROM        projovemurbano.transferencia tra
            INNER JOIN  projovemurbano.cadastroestudante cae ON cae.caeid = tra.cad_caeid
            WHERE 
                cae.pjuid = '".$_SESSION['projovemurbano']['pjuid']."'
                AND traid is not null";
    $checaTraid = $db->pegaUm($sql);
    
?>
<script language="javascript" type="text/javascript">
jQuery(document).ready(function() {
	jQuery('#caeid_combo_campo_off').find('select').removeClass('obrigatorio');
    jQuery('#marcar-todos').live('click',function(){
        jQuery('.doc').attr('checked', jQuery(this).attr('checked'));
        });
    
	jQuery('#modalidade').change(function(){
			jQuery('#formModalidade').submit();
		});
        
    function testaPossuiPolo( esfera, codigo, ppuid ){
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: "requisicao=testaPossuiPolo&esfera=" + esfera + "&codigo=" + codigo + "&ppuid=" + ppuid ,
            async: false,
            success: function(msg){
                top.test =  msg;
            }
        });

        return top.test;
    }
    jQuery('#pjuesfera, #ppuid_outros').change(function(){
        jQuery('#estuf').val('');
    });
    
    jQuery('#ppuid_outros, #pjuesfera, #estuf, #muncod').live('change',function(){
    
        var ppuid       = jQuery('#ppuid_outros').val();
        var pjuesfera   = jQuery('#pjuesfera').val();
        var estuf       = jQuery('#estuf').val();
        var dados       = '';
        var td          = '';
        
        if( ppuid == '' || pjuesfera == '' || estuf == '' ){
            jQuery('#td_polo').html('');
            jQuery('#td_nucleo').html('');
            return false;
        }
        if( pjuesfera == 'E' ){
            var pmupossuipolo2 = testaPossuiPolo( pjuesfera, estuf, ppuid );
            if( pmupossuipolo2 == 't' ){
                jQuery('#td_polo').show();
                dados   = "requisicao=buscarPolo&ppuid=" + ppuid + "&estuf=" + estuf ;
                td      = '#td_polo';
                jQuery('#tr_polo').show();
            }else{
                jQuery('#tr_polo').hide();
                jQuery('#td_polo').hide();
                jQuery('#td_polo').html('');
                dados   = "requisicao=buscarNucleo&com_vaga=true&pmupossuipolo=false&ppuid=" + ppuid + "&estuf=" + estuf ;
                td      = '#td_nucleo';
            }
        }else{
            var muncod = jQuery('#muncod').val();
            if( muncod == '' ){
                //jQuery('#tr_polo').hide();
                jQuery('#td_polo').html('');
                jQuery('#td_nucleo').html('');
                return false;
            }
            var pmupossuipolo2 = testaPossuiPolo( pjuesfera, muncod, ppuid );
            if( pmupossuipolo2 == 't' ){
                jQuery('#td_polo').show();
                dados   = "requisicao=buscarPolo&ppuid=" + ppuid + "&muncod=" + muncod ;
                td      = '#td_polo';
                jQuery('#tr_polo').show();
            }else{
                jQuery('#tr_polo').hide();
                jQuery('#td_polo').html('');
                dados   = "requisicao=buscarNucleo&com_vaga=true&pmupossuipolo=false&ppuid=" + ppuid + "&muncod=" + muncod ;
                td      = '#td_nucleo';
            }
        }
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: dados+'&tipo=O',
            async: false,
            success: function(msg){
                jQuery(td).html(msg);
            }
        });
        
    });
    
//    jQuery('#muncod').live('change',function(){
//        var muncod = jQuery(this).val();
////        jQuery('#td_polo').load(window.location.href + "requisicao=buscarPolo&ppuid=" + jQuery('#ppuid_outros').val() + "&muncod=" + jQuery('#muncod').val() + "&bloq=<?=$bloq ?>");
////        return false;
//        jQuery.ajax({
//            type: "POST",
//            url: window.location.href,
//            data: "requisicao=buscarPolo&ppuid=" + jQuery('#ppuid_outros').val() + "&muncod=" + jQuery('#muncod').val() + "&bloq=<?=$bloq ?>",
//            async: false,
//            success: function(msg){
//                jQuery('#td_polo').html(msg);
//                jQuery('#tr_polo').show();
//            }
//        });
//    });
	
	jQuery('#ppuid').change(function(){
	
		var ppuid = jQuery(this).val();
        var pmupossuipolo = jQuery('#pmupossuipolo').val();
		if( pmupossuipolo == 't' ){
			jQuery.ajax({
		 		type: "POST",
		      	url: window.location.href,
		     	data: "requisicao=buscarPolo&ppuid=" + ppuid + "&bloq=<?=$bloq ?>",
		      	async: false,
		      	success: function(msg){
					jQuery('#td_polo').html(msg);
		      	}
		    });
		}else{ 
			jQuery.ajax({
		 		type: "POST",
		      	url: window.location.href,
		     	data: "requisicao=buscarNucleo&pmupossuipolo=false&ppuid=" + ppuid + "&bloq=<?=$bloq ?>",
		      	async: false,
		      	success: function(msg){
					jQuery('#td_nucleo1').html(msg);
		      	}
		    });
		}
	});
    
    jQuery('#pjuesfera').change(function(){
        
        var pjuesfera = jQuery(this).val();
        if( pjuesfera == 'E' ){
            jQuery('#tr_muncod').hide();
            var html = '<select id="muncod" style="width: auto" class="CampoEstilo" name="muncod"><option value="">Selecione uma UF</option></select>';
            jQuery('#td_muncod').html(html);
        }else{
            jQuery('#tr_muncod').show();
        }
    });
	
	jQuery('#caecpf').blur(function(){
	
		var pmupossuipolo = jQuery('#pmupossuipolo').val();
		
		var caecpf = replaceAll(replaceAll(jQuery('#caecpf').val(),'.',''),'-','');
		
		if( caecpf != '' ){
			jQuery.ajax({
		 		type: "POST",
		      	url: window.location.href,
		     	data: "requisicao=buscarDadosAluno&caecpf=" + caecpf + "&pmupossuipolo=" + pmupossuipolo,
		      	async: false,
		      	dataType: 'json',
		      	success: function(msg){
			      	if( msg['nome'] !="Aluno não encontrado, o aluno deve ser da mesma coordenação e do mesmo ano de execução do sistema que está operando" ){
// 				      	alert('entrou');

			      		jQuery('#caeid').val(msg['caeid']);
			      		jQuery('#turidorigem').val(msg['turidorigem']);
                        jQuery('#anoorigem').val(msg['anoorigem']);
			      		jQuery('#dataingresso').val(msg['dataingresso']);
                        jQuery('#polori').val(msg['polid']);
                        jQuery('#nucori').val(msg['nucid']);
						jQuery('#td_nome').html(msg['nome']);
						if( pmupossuipolo == 't' ){
							jQuery('#td_polori').html(msg['polo']);
						}else{
							jQuery('#td_polori').html('Não possui polo');
						}console.log(msg);
						jQuery('#td_nucori').html(msg['nucleo']);
						jQuery('#td_turmaori').html(msg['turma']);
			      	}else{
			      		jQuery('#td_nome').html(msg['nome']);
			      		jQuery('#caeid').val(msg['']);
			      		jQuery('#turidorigem').val(msg['']);
                        jQuery('#anoorigem').val(msg['']);
			      		jQuery('#dataingresso').val(msg['']);
                        jQuery('#nucori').val(msg['']);
                        jQuery('#caecpf').val('');
                        if( pmupossuipolo == 'f' ){
                            jQuery('#polori').val('');
                        }
						jQuery('#td_nome').html(msg['nome']);
						if( pmupossuipolo == 'f' ){
							jQuery('#td_polori').html('');
						}
						jQuery('#td_nucori').html('');
						jQuery('#td_turmaori').html('');
			      	}
                }
		    });
		}
	});
	
	jQuery('.salvar').click(function(){
		// Valida campos obrigatórios
		var erro = false;
		jQuery('.obrigatorio').each(function(){
			if( jQuery(this).val() == '' ){
				var idCampo = jQuery(this).attr('id');
				var nomeCampo = jQuery(this).parent().prev().text().replace(/[^a-záéíóúãõâêîôûç \/]/gi,'');
				alert("O campo '"+ nomeCampo +"' não pode ser deixado em branco.");
				rolaTela(idCampo);
				jQuery(this).focus();
				erro = true;
				return false;
			}
		});
	
		if( erro ){
			return false;
		}
		
        if( jQuery('#modo').val() == 'M' ){
        	
            if( jQuery('#caeid_combo').children('[value!=""]').length < 1 ){
                alert('Selecione pelo menos 1 aluno');
                return false;
            }
            jQuery('#caeid_combo option').attr('selected',true);
            if(jQuery('#turidM').val()==jQuery('#turid').val()){
            	alert('A turma de destino deve ser diferente da turma de origem.');
				return false;
            }
        }else{
        	if(jQuery('#caecpf').val()==''){
        		alert('Preencha o campo cpf.');
        		return false
        	}
        	  if(jQuery('#turidorigem').val()==jQuery('#turid').val()){
              	alert('A turma de destino deve ser diferente da turma de origem.');
  				return false;
              }
        }
		// FIM Valida campos obrigatórios
		
		jQuery('#requisicao').val('enviar_aluno_salvar');
		jQuery('#form').submit();
	});
    
    jQuery('.multipla').click(function(){
        jQuery('#selecao_multipla').show();
        jQuery('#selecao_simples').hide();
        jQuery('#modo').val('M');        
        jQuery('#caeid_combo').addClass('obrigatorio');
        jQuery('#caecpf').removeClass('obrigatorio');
    });
    
    jQuery('.simples').click(function(){
        jQuery('#selecao_multipla').hide();
        jQuery('#selecao_simples').show();
        jQuery('#modo').val('S');
        jQuery('#caecpf').addClass('obrigatorio');
        jQuery('#caeid_combo').removeClass('obrigatorio');
    });
	
    jQuery('#polidM, #nucidM, #turidM').live('change',function(){
        jQuery('#tr_caeid_combo').remove();
        jQuery('#turidM').parent().parent().after('<tr><td colspan="2"> Carregando... </td></tr>');
        var polid = jQuery('#polidM').val();
        var nucid = jQuery('#nucidM').val();
        var turid = jQuery('#turidM').val();
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: "requisicao=buscarComboAlunos&polid=" + polid + "&nucid=" + nucid + "&turid=" + turid,
            async: false,
            success: function(msg){
                jQuery('#turidM').parent().parent().next().remove()
                jQuery('#turidM').parent().parent().after(msg);
            }
        });
    });
    
    jQuery('.excluir').live('click',function(){
        var traid = jQuery(this).attr('id');
        jQuery('#requisicao').val('enviar_aluno_cancelarTranferencia');
        jQuery('#traid').val(traid);
        jQuery('#form').submit();
    });
    
    jQuery('.transferir').click(function(){
        if( jQuery('[name="traid[]"]:checked').length < 1 ){
            alert('Seleciona pelo menos uma tranferência.');
            return false;
        }
        jQuery('#form_transferencias').submit();
    });
    
    jQuery('.baixar').click(function(){
        jQuery('#arqid').val( jQuery(this).attr('id') );
        jQuery('#form').submit();
    });
});

 
function carregarMunicipios2(estuf,muncod) {
    var pjuesfera = jQuery('#pjuesfera').val();
    estuf = jQuery('#estuf').val();
    if( pjuesfera == 'M' && estuf != '' ){
        jQuery('#tr_muncod').show();
        jQuery('#td_muncod').html('Carregando...');
        jQuery.ajax({
            type: "POST",
            url: window.location,
            data: "requisicao=carregarMunicipios2&funcao={semfuncao}&estuf="+estuf,
            async: false,
            success: function(msg){jQuery('#td_muncod').html(msg);}
        });
    }
}
 
     
function buscarNucleoM(polid, bloq) {
  if (polid != '') {
    document.getElementById('td_nucleoM').innerHTML = 'Carregando...';
    jQuery.ajax({
      type: "POST",
      url: window.location.href,
      data: "requisicao=buscarNucleos&form=M&ppuid=<?=$_SESSION['projovemurbano']['ppuid']?>&polid=" + polid + "&bloq=<?=$bloq ?>",
      async: false,
      success: function(msg) {
        document.getElementById('td_nucleoM').innerHTML = msg;
      }
    });
  }
}

function buscarNucleo(polid) {

  if (polid != '') {
  <? if($_POST['modalidade'] == 'M'){?>
        var ppuid       = jQuery('#ppuid').val();
  <?}else{?>
        var ppuid       = jQuery('#ppuid_outros').val();
    <?}?>
    document.getElementById('td_nucleo').innerHTML = 'Carregando...';
    jQuery.ajax({
      type: "POST",
      url: window.location.href,
      data: "requisicao=buscarNucleo&com_vaga=true&pmupossuipolo=t&polid=" + polid + "&ppuid=" + ppuid + "&bloq=<?=$bloq ?>",
      async: false,
      success: function(msg) {

        document.getElementById('td_nucleo').innerHTML = msg;
      }
    });
  }
}

function buscarTurmasM(nucid, bloq) {
    if (nucid != '') {
        document.getElementById('td_turmaM').innerHTML = 'Carregando...';
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: "requisicao=buscarTurmas&form=M&nucid=" + nucid + "&bloq=<?=$bloq ?>",
            async: false,
            success: function(msg){document.getElementById('td_turmaM').innerHTML = msg;}
        });
    }
}

function buscarTurmas(nucid, bloq) {
  if (nucid != '') {
    document.getElementById('td_turma').innerHTML = 'Carregando...';
    jQuery.ajax({
      type: "POST",
      url: "projovemurbano.php?modulo=principal/monitoramento2013&acao=A",
      data: "requisicao=buscarTurmas&com_vaga=true&nucid=" + nucid + "&bloq=<?=$bloq ?>",
      async: false,
      success: function(msg){document.getElementById('td_turma').innerHTML = msg;}
    });
  }
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
    var div_on = document.getElementById( campo + '_campo_on' );
    var div_off = document.getElementById( campo + '_campo_off' );
    var input = document.getElementById( campo + '_campo_flag' );
    if ( div_on.style.display == 'none' )
    {
        div_on.style.display = 'block';
        div_off.style.display = 'none';
        input.value = '1';
    }
    else
    {
        div_on.style.display = 'none';
        div_off.style.display = 'block';
        input.value = '0';
    }
}

</script>
<style>
.div_muda{
align:top;
}
</style>

<?
if($_SESSION['projovemurbano']['ppuid']!=1){
	$filtrotprid = "AND tprid = '".$_SESSION['projovemurbano']['tprid']."'";
}
$sql = "SELECT * FROM projovemurbano.polomunicipio WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."' $filtrotprid AND pmustatus='A'";
$polomunicipio = $db->pegaLinha($sql);
?>	
<form id="formModalidade" name="formModalidade" method="POST">
	<center>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
			<tr>
				<td class="SubTituloEsquerda" width="10%">Transferir o aluno para:</td>
				<td id="td_modalidade">
				<?php
					$arrModalidade = array( 
										array("codigo" => "M", "descricao" => "Mesma Coordenação"), 
										array("codigo" => "O", "descricao" => "Outra Coordenação")
					                                                      ); 
	                                                      
					$_SESSION['projovemurbano']['modalidade'] = $_POST['modalidade']!= ''?$_POST['modalidade']:$_SESSION['projovemurbano']['modalidade'];                                                      
					$modalidade = $_SESSION['projovemurbano']['modalidade'];
					$db->monta_combo('modalidade', $arrModalidade, 'S', 'Selecione a Modalidade', '', '', '', '150', 'S', 'modalidade');
					 
				?>
				</td>
			</tr>
		</table>
	</center>
</form>
<?if($_SESSION['projovemurbano']['modalidade']){?>
<form id="form" enctype="multipart/form-data" name="form" method="POST" >
	<input type="hidden" value="" name="requisicao" id="requisicao" />
    <input type="hidden" value="S" name="modo" id="modo" />
    <input type="hidden" value="" name="traid" id="traid" />
	<input type="hidden" value="" name="arqid" id="arqid" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="Center" border="0">
		<tr>
			<td class="SubTituloCentro" width="50%" colspan="2">Dados Origem</td>
        </tr>
        <tr>
			<td colspan="2">
				<div id="selecao_simples" style="align:top">
                <?if($_SESSION['projovemurbano']['modalidade'] == 'M'){?>
                    <div class="div_muda">
                        <a class="multipla" style="cursor:pointer">Seleção Múltipla</a>
                        <input type="hidden" value="1" name="multipla" id="multipla" />
                    </div>
                <?}?>
					<table width="100%"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="Center" border="0">
						<tr>
							<td class="SubTituloDireita" width="25%">CPF:
                                <input type="hidden" id="pmupossuipolo" value="<?=$polomunicipio['pmupossuipolo'] ?>" />
                            </td>
							<td width="75%">
                                <input type="hidden" name="anoorigem" id="anoorigem" value=""/>
                                <input type="hidden" name="polori" id="polori" value=""/>
                                <input type="hidden" name="nucori" id="nucori" value=""/>
                                <input type="hidden" name="turidorigem" id="turidorigem" value=""/>
                                <input type="hidden" name="dataingresso" id="dataingresso" value=""/>
                                <input type="hidden" name="caeid" id="caeid" value=""/>
                                <? echo campo_texto('caecpf', 'N','S', "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="caecpf" required', '', '', ');' ); ?>
                            </td>
						</tr>
						<tr>
							<td class="SubTituloDireita" >Nome:</td>
							<td id="td_nome"></td>
						</tr>
						<tr>
							<td class="SubTituloDireita">Pólo de Origem:</td>
							<td id="td_polori">
							</td>
						</tr>
						<tr>
							<td class="SubTituloDireita" width="15%">Núcleo de Origem:</td>
							<td width="15%" id="td_nucori"><? ?></td>
						</tr>
						<tr>
							<td class="SubTituloDireita" width="15%">Turma de origem:</td>
							<td width="15%" id="td_turmaori"></td>
						</tr>
					</table>
				</div>
				<div id="selecao_multipla" style="display:none;align:top">
                    <div class="div_muda">
                        <a class="simples" style="cursor:pointer">Seleção Simples</a>
                    </div>
                    <table width="100%"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="0">
                        <? if( $polomunicipio['pmupossuipolo'] == 't' ){ ?>
                        <tr>
                            <td class="SubTituloDireita" width="25%" >Pólo:</td>
                            <td id="td_poloM">
                                <?
                                    $_REQUEST['ppuid'] = $_SESSION['projovemurbano']['ppuid'];
                                    $_REQUEST['form'] = 'M';
                                    buscarPolo( );
                                    unset( $_REQUEST['form']);
                                ?>
                            </td>
                        </tr>
                        <? } ?>
                        <tr>
                            <td class="SubTituloDireita" width="15%">Núcleo:</td>
                            <td id="td_nucleoM">
                                <? 
                                if( $polomunicipio['pmupossuipolo'] != 't' ){
                                    $_REQUEST['ppuid'] = $_SESSION['projovemurbano']['ppuid'];
                                    $_REQUEST['form'] = 'M';
                                    buscarNucleo( );
                                    unset( $_REQUEST['form']);
                                }else{
                                    $db->monta_combo('nucidM', array(), 'S', 'Selecione um polo', '', '', '', '', 'N', 'nucidM'); 
                                }
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita" >Turma:</td>
                            <td id="td_turmaM"><? $db->monta_combo('turidM', array(), 'S', 'Selecione um nucleo', '', '', '', '', 'N', 'turidM');  ?></td>
                        </tr>
                        <?
                       buscarComboAlunos()
                        ?>
                    </table>
				</div>
			</td>
        </tr>
        <tr>
            <td class="SubTituloCentro" width="50%" colspan="2">Dados do Destino</td>
        </tr>
        <tr>
			<td>
				<table width="100%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="Center" border="0">
				<? if($modalidade == 'M'){?>
					<tr>
						<td class="SubTituloDireita" width="25%">Ano de Execução do Projeto:</td>
						<td width="75%" colspan="5">
						<? 
							anoDestino();
						?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" >Transferir para o Pólo:</td>
						<td id="td_polo">
						<?
						if( $polomunicipio['pmupossuipolo'] == 't' ){
							if($polid){ ?>
							buscarPolo();
							<script>
							jQuery(document).ready(function() {
								buscarNucleo('<?=$polid ?>');
								jQuery('#nucid').val('<?=$nucid ?>');
								buscarTurmas('<?=$nucid ?>');
								jQuery('#caeturma').val('<?=$caeturma ?>');
							});
							</script>
							<?
							}elseif( $ppuid != '' ){
								buscarPolo();
							}else{
								$db->monta_combo('polid', array(), 'S', 'Selecione um ano de execução', '', '', '', '', 'S', 'polid');
							}
						}else{
							echo "Não possui polo";
						}
						?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="15%">Transferir para o Núcleo:</td>
							<? 
							if($polid==''&&$nucid){ 
							?>
							<script>
							jQuery(document).ready(function() {
								buscarTurmas('<?=$nucid ?>','S');
								jQuery('#caeturma').val('<?=$caeturma ?>');
							});
							</script>
							<? 
							}
							if( $polomunicipio['pmupossuipolo'] == 't' ){?>
						<td id="td_nucleo">
							<? if($nucid!=''){ 
									if( contaEstudantesNucleos($polomunicipio['pmupossuipolo']) ){
										$db->monta_combo('caenucleo', array(), 'S', 'Numero máximo de estudantes atingido.', '', '', '', '', 'S', 'caeturma');
									}else{
										buscarNucleo();
									}
								}else{ 
									$db->monta_combo('caenucleo', array(), 'S', 'Selecione um polo', '', '', '', '', 'S', 'caeturma');
								}  
							?>
						<? 	}else{?>
						<td id="td_nucleo1">
						<?	
								if($nucid!=''){ 
									if( contaEstudantesNucleos($polomunicipio['pmupossuipolo']) ){
										$db->monta_combo('caenucleo', array(), 'S', 'Numero máximo de estudantes atingido.', '', '', '', '', 'S', 'caeturma');
									}else{
										buscarNucleo();
									}
								}else{ 
									$db->monta_combo('caenucleo', array(), 'S', 'Selecione um ano de execução', '', '', '', '', 'S', 'caeturma');
								} 
							}
						?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" >Transferir para a Turma:</td>
						<td id="td_turma"><? $db->monta_combo('caeturma', array(), 'S', 'Selecione um nucleo', '', '', '', '', 'S', 'caeturma');  ?></td>
					</tr>
					<tr>
						<td class="SubTituloDireita" width="15%">Documento de Pedido de Transferência:</td>
						<td>
							<input  type="file" name="arquivo" id="anexo" style="cursor:pointer"> 
						</td>
					</tr>
				<? }else{ ?>
                    <tr>
                        <td class="SubTituloDireita" width="25%">Ano de Execução do Projeto:</td>
                        <td width="75%" >
                        <? 
                            anoDestino();
                        ?>
                        </td>
                    </tr>
                    <tr>
						<td class="SubTituloDireita" >Esfera Destino:</td>
						<td>
							<? 
								$sql = "SELECT DISTINCT 
											CASE 
												WHEN estuf IS NULL  THEN 'M' 
												WHEN muncod IS NULL THEN 'E' 
											END as codigo, 
											CASE 
												WHEN estuf IS NULL  THEN 'Municipal' 
												WHEN muncod IS NULL THEN 'Estadual' 
											END as descricao
										FROM 
											projovemurbano.projovemurbano";
								
								$db->monta_combo('pjuesfera', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'pjuesfera'); 
							?>
						</td>
                    </tr>
                    <tr>
						<td class="SubTituloDireita">UF Destino:</td>
						<td id="td_uf">	
                            <? 
                            $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";

                            $db->monta_combo('estuf', $sql, $bloq, 'Selecione a esfera', 'carregarMunicipios2', '', '', '', 'S', 'estuf'); 
                            ?>
						</td>
                    </tr>
                    <tr id="tr_muncod">
                        <td class="SubTituloDireita">Munícipio</td>
                        <td id="td_muncod">
                            <? $db->monta_combo('muncod', array(), $bloq, 'Selecione uma UF', '', '', '', '', 'N', 'muncod'); ?>
                        </td>
                    </tr>
                    <tr id="tr_polo">
                        <td class="SubTituloDireita" >Transferir para o Pólo:</td>
                        <td id="td_polo">
                        <? $db->monta_combo('polid', array(), 'S', 'Selecione um ano de execução', '', '', '', '', 'S', 'polid'); ?>
                        </td>
                    </tr>
                    <tr id="tr_nucleo">
                        <td class="SubTituloDireita" width="15%">Transferir para o Núcleo:</td>
                        <td id="td_nucleo" >
                            <? if($nucid!=''){ 
                                    if( contaEstudantesNucleos($polomunicipio['pmupossuipolo']) ){
                                        $db->monta_combo('caenucleo', array(), 'S', 'Numero máximo de estudantes atingido.', '', '', '', '', 'S', 'caeturma');
                                    }else{
                                        buscarNucleo();
                                    }
                                }else{ 
                                    $db->monta_combo('caenucleo', array(), 'S', 'Selecione um polo', '', '', '', '', 'S', 'caeturma');
                                }  
                            ?>
                        </td>
                    </tr>
					<tr>
						<td class="SubTituloDireita" >Documento de pedido de transferência:</td>
						<td>
                            <input  type="file" name="arquivo" id="anexo" style="cursor:pointer"> 
                        </td>
					</tr>
					<tr>
						<td class="SubTituloDireita" >Texto Adicional:</td>
						<td width="15%">
                            <?=campo_textarea("justificativa","N","S","Digite o outro motivo", 60, 3, 200,"","","","","")?>
                        </td>
					</tr>
				<? } ?>
				</table>
			</td>
		</tr>
		<tr>
        <?if(!in_array ( PFL_CONSULTA, $perfis )
//         		||$permiteTransferencia =='t'
				){?>
			<td class="SubTituloCentro" colspan="4">
				<input type="button" class="salvar" value="Inserir">
			</td>
      <?}?>
		</tr>
	</table>
</form>
<?
monta_titulo('Alunos a transferir','');

$check = '<input type="checkbox" id="marcar-todos" >';  

$where = array("hst.shtid_status = 1");

if(!$db->testa_superuser()) {
    $perfis = pegaPerfilGeral();
//     $where[] = "tra.usucpfresponsavelorigem = '{$_SESSION['usucpf']}'";
//     $where[] = "tra.ppuid_origem = {$_SESSION['projovemurbano']['ppuid']}";
}

if( in_array(PFL_COORDENADOR_ESTADUAL, $perfis) ){
    $inner_local = "INNER JOIN territorios.estado e ON e.estuf = pro.estuf";
}elseif(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
    $inner_local = "INNER JOIN territorios.municipio m ON m.muncod = pro.muncod";
}
    
$sqlano = " SELECT
                ppuano as ano2
             FROM
                projovemurbano.programaprojovemurbano ppu
            WHERE ";
// ver($_SESSION['projovemurbano']['pjuid'],d);
if($_SESSION['projovemurbano']['modalidade'] == 'M'){   
    
    $where[] = " tra.turid_destino IS NOT NULL ";
    
    $sql = "SELECT DISTINCT
                '<center><input type= \"checkbox\" class=\"doc\" name=\"traid[]\" value=\"'|| tra.traid ||'\" /></center>' as marcar,
                '<center><img src=../imagens/excluir.gif border=0 width=13px
                      title=Excluir style=cursor:pointer; 
                      id='|| tra.traid ||' class=excluir /></center>' as excluir, 
                cae.caecpf as cpf,
                cae.caenome as nome,
                ( coalesce(estO.estuf,munO.estuf||' - '||munO.mundescricao) ) as localorigem,
                'POLO '||tra.polid_origem as poloOrigem,
                'NÚCLEO '|| tra.nucid_origem as nucleoOrigem,
                (SELECT DISTINCT 
                    turdesc AS descricao
                FROM projovemurbano.turma 
                WHERE  turid = tra.turid_origem
                --AND    turstatus = 'A'
				) AS turmaorigem,
                ($sqlano ppu.ppuid = tra.ppuid_origem) as AnoOrigem,
                ( coalesce(est.estuf,mun.estuf||' - '||mun.mundescricao) ) as localdestino,
                'POLO '||tra.polid_destino as poloDestino,
                'NÚCLEO '|| tra.nucid_destino as nucleoDestino,
                (SELECT DISTINCT 
                    turdesc AS descricao
                FROM projovemurbano.turma 
                WHERE  turid = tra.turid_destino
                --AND    turstatus = 'A'
				) AS turmadestino,
                ($sqlano ppu.ppuid = tra.ppuid_destino) as AnoDestino,
                CASE WHEN arqt.arqid IS NULL
                    THEN 'Não Existente'
                    ELSE '<center><a id='||arqt.arqid||' style=\"cursor:pointer;color:blue\" class=baixar>Baixar</a></center>'
                END as arquivo
            FROM 
                projovemurbano.transferencia tra 
            INNER JOIN projovemurbano.cadastroestudante cae ON cae.caeid = tra.cad_caeid 
    		LEFT JOIN projovemurbano.arquivo_tranferencia arqt ON arqt.traid = tra.traid
            INNER JOIN projovemurbano.historico_transferencia hst ON hst.htrid = tra.htrid_ultimo
            
            LEFT  JOIN projovemurbano.polo                          polO
            	INNER JOIN projovemurbano.polomunicipio             pmuO  ON pmuO.pmuid = polO.pmuid                          
        	ON polO.polid  = tra.polid_origem
	        LEFT  JOIN projovemurbano.nucleo                        nucO  
	            INNER JOIN projovemurbano.municipio                 muniO  ON muniO.munid = nucO.munid
	            INNER JOIN projovemurbano.polomunicipio             pmuO2 ON pmuO2.pmuid = muniO.pmuid  
	        ON nucO.nucid  = tra.nucid_origem 
	        LEFT  JOIN projovemurbano.projovemurbano                pjuO ON pjuO.pjuid = pmuO.pjuid OR pjuO.pjuid = pmuO2.pjuid
	        LEFT  JOIN territorios.municipio                        munO ON munO.muncod = pjuO.muncod
	        LEFT  JOIN territorios.estado                           estO ON estO.estuf = pjuO.estuf
	    
	        LEFT  JOIN projovemurbano.polo pol
	            INNER JOIN projovemurbano.polomunicipio             pmu  ON pmu.pmuid = pol.pmuid                          
	        ON pol.polid  = tra.polid_destino
	        LEFT  JOIN projovemurbano.nucleo                        nuc  
	            INNER JOIN projovemurbano.municipio                 muni  ON muni.munid = nuc.munid
	            INNER JOIN projovemurbano.polomunicipio             pmu2 ON pmu2.pmuid = muni.pmuid  
	        ON nuc.nucid  = tra.nucid_destino
	        LEFT  JOIN projovemurbano.projovemurbano                pju ON pju.pjuid = pmu.pjuid OR pju.pjuid = pmu2.pjuid
	        LEFT  JOIN territorios.municipio                        mun ON mun.muncod = pju.muncod
	        LEFT  JOIN territorios.estado                           est ON est.estuf = pju.estuf
            WHERE
            	tra.ppuid_origem = {$_SESSION['projovemurbano']['ppuid']} 
            AND   tra.pjuid_origem = {$_SESSION['projovemurbano']['pjuid']} AND
                 ".implode(' AND ', $where)." 
            ORDER BY
                nome";
//    ver($sql,d);
        $cabecalho = array($check,"Excluir","CPF Aluno","Aluno","Local Origem","Pólo Origem","Núcleo Origem","Turma Origem","Ano Origem","Local Destino","Pólo Destino","Núcleo Destino","Turma Destino","Ano destino","Arquivo Anexo");
                
 }elseif($_SESSION['projovemurbano']['modalidade'] == 'O'){
    
    $where[] = " tra.turid_destino IS NULL ";
    
    $sql = "SELECT DISTINCT
                '<center><input type=\"checkbox\" class=\"doc\" name=\"traid[]\" value=\"'|| tra.traid ||'\" /></center>' as marcar,
                '<center><img src=../imagens/excluir.gif border=0 width=13px
                      title=Excluir style=cursor:pointer; 
                      id='|| tra.traid ||' class=excluir /></center>' as excluir, 
                cae.caecpf as cpf,
                cae.caenome as nome,
                ( coalesce(estO.estuf,munO.estuf||' - '||munO.mundescricao) ) as localorigem,
                'POLO '||tra.polid_origem as poloOrigem,
                'NÚCLEO '|| tra.nucid_origem as nucleoOrigem,
                (SELECT DISTINCT 
                    turdesc AS descricao
                FROM projovemurbano.turma 
                WHERE  turid = tra.turid_origem
                --AND    turstatus = 'A'
        		) AS turmaorigem,
                ($sqlano ppu.ppuid = tra.ppuid_origem) as AnoOrigem,
                ( coalesce(est.estuf,mun.estuf||' - '||mun.mundescricao) ) as localdestino,
                'POLO '||tra.polid_destino as poloDestino,
                'NÚCLEO '|| tra.nucid_destino as nucleoDestino,
                (SELECT DISTINCT 
                    turdesc AS descricao
                FROM projovemurbano.turma 
                WHERE  turid = tra.turid_destino
                --AND    turstatus = 'A'
        		) AS turmadestino,
                ($sqlano ppu.ppuid = tra.ppuid_destino) as AnoDestino,
                CASE WHEN arqt.arqid IS NULL
                    THEN 'Não Existente'
                    ELSE '<center><a id='||arqt.arqid||' style=\"cursor:pointer;color:blue\" class=baixar>Baixar</a></center>'
                END as arquivo 
            FROM 
                projovemurbano.transferencia tra 
            INNER JOIN projovemurbano.cadastroestudante         cae     ON cae.caeid  = tra.cad_caeid 
            LEFT  JOIN projovemurbano.arquivo_tranferencia      arqt    ON arqt.traid = tra.traid
            INNER JOIN projovemurbano.historico_transferencia   hst     ON hst.htrid  = tra.htrid_ultimo
            
            LEFT  JOIN projovemurbano.polo                          polO
            	INNER JOIN projovemurbano.polomunicipio             pmuO  ON pmuO.pmuid = polO.pmuid                          
        	ON polO.polid  = tra.polid_origem
	        LEFT  JOIN projovemurbano.nucleo                        nucO  
	            INNER JOIN projovemurbano.municipio                 muniO  ON muniO.munid = nucO.munid
	            INNER JOIN projovemurbano.polomunicipio             pmuO2 ON pmuO2.pmuid = muniO.pmuid  
	        ON nucO.nucid  = tra.nucid_origem 
	        LEFT  JOIN projovemurbano.projovemurbano                pjuO ON pjuO.pjuid = pmuO.pjuid OR pjuO.pjuid = pmuO2.pjuid
	        LEFT  JOIN territorios.municipio                        munO ON munO.muncod = pjuO.muncod
	        LEFT  JOIN territorios.estado                           estO ON estO.estuf = pjuO.estuf
	    
	        LEFT  JOIN projovemurbano.polo pol
	            INNER JOIN projovemurbano.polomunicipio             pmu  ON pmu.pmuid = pol.pmuid                          
	        ON pol.polid  = tra.polid_destino
	        LEFT  JOIN projovemurbano.nucleo                        nuc  
	            INNER JOIN projovemurbano.municipio                 muni  ON muni.munid = nuc.munid
	            INNER JOIN projovemurbano.polomunicipio             pmu2 ON pmu2.pmuid = muni.pmuid  
	        ON nuc.nucid  = tra.nucid_destino
	        LEFT  JOIN projovemurbano.projovemurbano                pju ON pju.pjuid = pmu.pjuid OR pju.pjuid = pmu2.pjuid
	        LEFT  JOIN territorios.municipio                        mun ON mun.muncod = pju.muncod
	        LEFT  JOIN territorios.estado                           est ON est.estuf = pju.estuf
            WHERE 
            	tra.ppuid_origem = {$_SESSION['projovemurbano']['ppuid']}  
            AND   tra.pjuid_origem = {$_SESSION['projovemurbano']['pjuid']} AND
                ".implode(' AND ', $where)." 
            ORDER BY
                nome";
                
    $cabecalho = array($check,"Excluir","CPF Aluno","Aluno","Local Origem","Pólo Origem","Núcleo Origem","Turma Origem","Ano Origem","Local Destino","Pólo Destino","Núcleo Destino","Turma Destino","Ano destino","Arquivo Anexo");
 }	
// ver($sql,d);	
    echo "<form name=form_transferencias id=form_transferencias method=POST>";
    $db->monta_lista_simples($sql, $cabecalho, 100, 5, '', '95%', '', '');
    echo "</form>";
 ?>
 <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="0">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">
		 <?if(!in_array ( PFL_CONSULTA, $perfis )
				){?>
		  <input type="button" class="transferir" value="Transferir">
		  <?php }?>	
		</td>
	</tr>
 </table>
<?}?>
<? registarUltimoAcesso(); ?>
