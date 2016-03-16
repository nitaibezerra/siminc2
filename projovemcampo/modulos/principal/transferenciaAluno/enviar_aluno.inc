<?php
monta_titulo('','Dados da Transferência');
$_SESSION['projovemcampo']['modalidade'] = '';
$bloq = 'S'
?>
<script language="javascript" type="text/javascript">
jQuery(document).ready(function() {
	jQuery('#estid_combo_campo_off').find('select').removeClass('obrigatorio');
    jQuery('#marcar-todos').live('click',function(){
        jQuery('.doc').attr('checked', jQuery(this).attr('checked'));
        });
    
	jQuery('#modalidade').change(function(){
			jQuery('#formModalidade').submit();
		});
        
    jQuery('#apcesfera, #aprid_outros').change(function(){
        jQuery('#estuf').val('');
    });
    
    jQuery('#aprid_outros, #apcesfera, #estuf, #muncod').live('change',function(){
    
        var aprid       = jQuery('#aprid_outros').val();
        var apcesfera   = jQuery('#apcesfera').val();
        var estuf       = jQuery('#estuf').val();
        var dados       = '';
        var td          = '';
        if( aprid == '' || apcesfera == '' || estuf == '' ){
            return false;
		}
     
        if( apcesfera == 'E' ){
			if( aprid == '' || apcesfera == '' || estuf == '' ){
             jQuery('#td_escola').html('');
               	return false;
			}
			dados   = "requisicao=buscarescolaDestino&modalidade=<?=$_POST['modalidade'] ?>&aprid=" + aprid + "&estuf=" + estuf ;
			td      = '#td_escola';            
        }else{
            var muncod = jQuery('#muncod').val();
            if( aprid == '' || apcesfera == '' || muncod == '' ){
             jQuery('#td_escola').html('');
             return false;
			}
			dados   = "requisicao=buscarescolaDestino&modalidade=<?=$_POST['modalidade'] ?>&aprid=" + aprid + "&muncod=" + muncod ;
			td      = '#td_escola';
           
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
    
	jQuery('#aprid').change(function(){
	
		var aprid = jQuery(this).val();
       if(aprid!=''){
			jQuery.ajax({
		 		type: "POST",
		      	url: window.location.href,
		     	data: "requisicao=buscarescolaDestino&aprid=" + aprid + "&modalidade=<?=$_POST['modalidade'] ?>&bloq=<?=$bloq ?>",
		      	async: false,
		      	success: function(msg){
					jQuery('#td_escola1').html(msg);
		      	}
		    });
       }
	});
    
    jQuery('#apcesfera').change(function(){
        
        var apcesfera = jQuery(this).val();
        if( apcesfera == 'E' ){
            jQuery('#tr_muncod').hide();
            var html = '<select id="muncod" style="width: auto" class="CampoEstilo" name="muncod"><option value="">Selecione uma UF</option></select>';
            jQuery('#td_muncod').html(html);
        }else{
            jQuery('#tr_muncod').show();
        }
    });
	
	jQuery('#estcpf').blur(function(){
	
		var estcpf = replaceAll(replaceAll(jQuery('#estcpf').val(),'.',''),'-','');
		
		if( estcpf != '' ){
			jQuery.ajax({
		 		type: "POST",
		      	url: window.location.href,
		     	data: "requisicao=buscarDadosAluno&estcpf=" + estcpf ,
		      	async: false,
		      	dataType: 'json',
		      	success: function(msg){
			      	if( msg['nome'] !="Aluno não encontrado, o aluno deve ser da mesma coordenação e do mesmo ano de execução do sistema que está operando" ){
// 				      	alert('entrou');

			      		jQuery('#estid').val(msg['estid']);
			      		jQuery('#turidorigem').val(msg['turidorigem']);
                        jQuery('#anoorigem').val(msg['anoorigem']);
			      		jQuery('#dataingresso').val(msg['dataingresso']);
						jQuery('#td_nome').html(msg['nome']);
						jQuery('#td_escolaorigem').html(msg['escola']);
						jQuery('#td_turmaorigem').html(msg['turma']);
			      	}else{
			      		jQuery('#estid').val(msg['']);
			      		jQuery('#turidorigem').val(msg['']);
                        jQuery('#anoorigem').val(msg['']);
			      		jQuery('#dataingresso').val(msg['']);
						jQuery('#td_nome').html(msg['']);
						jQuery('#td_escolaorigem').html(msg['']);
						jQuery('#td_turmaorigem').html(msg['']);
			      	}
                }
		    });
		}
	});
	
	jQuery('.salvar').click(function(){
		// Valida campos obrigatórios
		var erro = false;
// 		jQuery('.obrigatorio').each(function(){
// 			if( jQuery(this).val() == '' ){
// 				var idCampo = jQuery(this).attr('id');
// 				var nomeCampo = jQuery(this).parent().prev().text().replace(/[^a-záéíóúãõâêîôûç \/]/gi,'');
// 				alert("O campo '"+ nomeCampo +"' não pode ser deixado em branco.");
// 				rolaTela(idCampo);
// 				jQuery(this).focus();
// 				erro = true;
// 				return false;
// 			}
// 		});
	
		if( erro ){
			return false;
		}
        if( jQuery('#modo').val() == 'M' ){
        	
            if( jQuery('#estid_combo').children('[value!=""]').length < 1 ){
                alert('Selecione pelo menos 1 aluno');
                return false;
            }
            jQuery('#estid_combo option').attr('selected',true);
            if(jQuery('#turidM').val()==jQuery('#turidDestino').val()){
            	alert('A turma de destino deve ser diferente da turma de origem.');
				return false;
            }
            if(jQuery('#turidDestino').val()==''){
            	alert('Escolha uma turma de destino.');
  				return false;
            }
        }
        if(jQuery('#modo').val() == 'S' ){
        	if(jQuery('#estcpf').val()==''){
        		alert('Preencha o campo cpf.');
        		return false
        	}
        	  if(jQuery('#turidorigem').val()==jQuery('#turidDestino').val()){
              	alert('A turma de destino deve ser diferente da turma de origem.');
  				return false;
              }
            if(jQuery('#turidDestino').val()==''){
            	alert('Escolha uma turma de destino.');
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
        jQuery('#estid_combo').addClass('obrigatorio');
        jQuery('#estcpf').removeClass('obrigatorio');
    });
    
    jQuery('.simples').click(function(){
        jQuery('#selecao_multipla').hide();
        jQuery('#selecao_simples').show();
        jQuery('#modo').val('S');
        jQuery('#estcpf').addClass('obrigatorio');
        jQuery('#estid_combo').removeClass('obrigatorio');
    });
	
    jQuery('#turidM').live('change',function(){
        jQuery('#tr_estid_combo').remove();
        jQuery('#turidM').parent().parent().after('<tr><td colspan="2"> Carregando... </td></tr>');
        
        var turid = jQuery('#turidM').val();
        if(turid!=''){
	        jQuery.ajax({
	            type: "POST",
	            url: window.location.href,
	            data: "requisicao=buscarComboAlunos&turid=" + turid,
	            async: false,
	            success: function(msg){
	                jQuery('#turidM').parent().parent().next().remove()
	                jQuery('#turidM').parent().parent().after(msg);
	            }
	        });
        }
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
    var apcesfera = jQuery('#apcesfera').val();
    estuf = jQuery('#estuf').val();
    if( apcesfera == 'M' && estuf != '' ){
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

function buscarTurmasM(entid, bloq) {
    if (entid != '') {
        document.getElementById('td_turmaM').innerHTML = 'Carregando...';
        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: "requisicao=buscarTurmas&form=M&entid=" + entid + "&bloq=<?=$bloq ?>",
            async: false,
            success: function(msg){document.getElementById('td_turmaM').innerHTML = msg;}
        });
    }
}

function buscarTurmasDestino(entid, bloq) {
  if (entid != '') {
    document.getElementById('td_turma').innerHTML = 'Carregando...';
    jQuery.ajax({
      type: "POST",
      url: window.location.href,
      data: "requisicao=buscarTurmasDestino&com_vaga=true&entid=" + entid + "&bloq=<?=$bloq ?>",
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
	                                                      
					$_SESSION['projovemcampo']['modalidade'] = $_POST['modalidade']!= ''?$_POST['modalidade']:$_SESSION['projovemcampo']['modalidade'];                                                      
					$modalidade = $_SESSION['projovemcampo']['modalidade'];
					$db->monta_combo('modalidade', $arrModalidade, 'S', 'Selecione a Modalidade', '', '', '', '150', 'S', 'modalidade');
					 
				?>
				</td>
			</tr>
		</table>
	</center>
</form>
<?if($_SESSION['projovemcampo']['modalidade']){?>
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
                <?if($_SESSION['projovemcampo']['modalidade'] == 'M'){?>
                    <div class="div_muda">
                        <a class="multipla" style="cursor:pointer">Seleção Múltipla</a>
                        <input type="hidden" value="1" name="multipla" id="multipla" />
                    </div>
                <?}?>
					<table width="100%"class="tabela" bgcolor="#f5f5<input type="hidden" value="" name="requisicao" id="requisicao" />f5" cellSpacing="1" cellPadding="3"	align="Center" border="0">
						<tr>
							<td class="SubTituloDireita" width="25%">
								CPF:
                            </td>
							<td width="75%">
                                <input type="hidden" name="anoorigem" id="anoorigem" value=""/>
                                <input type="hidden" name="turidorigem" id="turidorigem" value=""/>
                                <input type="hidden" name="dataingresso" id="dataingresso" value=""/>
                                <input type="hidden" name="estid" id="estid" value=""/>
                                <? echo campo_texto('estcpf', 'N','S', "CPF", 16, 14, "###.###.###-##", "", '', '', 0, 'id="estcpf" required', '', '', ');' ); ?>
                            </td>
						</tr>
						<tr>
							<td class="SubTituloDireita" >Nome:</td>
							<td id="td_nome"></td>
						</tr>
						<tr>
							<td class="SubTituloDireita" width="15%">Escola de Origem:</td>
							<td width="15%" id="td_escolaorigem"><? ?></td>
						</tr>
						<tr>
							<td class="SubTituloDireita" width="15%">Turma de origem:</td>
							<td width="15%" id="td_turmaorigem"></td>
						</tr>
					</table>
				</div>
				<div id="selecao_multipla" style="display:none;align:top">
                    <div class="div_muda">
                        <a class="simples" style="cursor:pointer">Seleção Simples</a>
                    </div>
                    <table width="100%"class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="0">
                        <tr>
                            <td class="SubTituloDireita" width="15%">Escola:</td>
                            <td id="td_escolaM">
                                <? 
                                    buscarEscola();
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita" >Turma:</td>
                            <td id="td_turmaM"><? $db->monta_combo('turidM', array(), 'S', 'Selecione um escola', '', '', '', '', 'N', 'turidM');  ?></td>
                        </tr>
                        <?
							buscarComboAlunos();
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
						<td class="SubTituloDireita" width="15%">Transferir para o Escola:</td>
							<? 
							if($entidDestino){ 
							?>
								<script>
									jQuery(document).ready(function() {
										buscarTurmasDestino('<?=$entidDestino ?>','S');
										jQuery('#turmaDestino').val('<?=$turmaDestino?>');
									});
								</script>
							<? 
							}
							?>
						<td id="td_escola1">
						<?	
							if($aprid!=''){
								buscarEscolaDestino();
							}else{
								$db->monta_combo('entidDestino', array(), 'S', 'Selecione um ano de execução', '', '', '', '', 'S', 'entidDestino');
							}
						?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" >Transferir para a Turma:</td>
						<td id="td_turma">
							<?
								if($entidDestino!=''){
									buscarTurmasDestino($entidDestino);
								}else{ 
                                    $db->monta_combo('turidDestino', array(), 'S', 'Selecione uma Escola', '', '', '', '', 'S', 'turidDestino');
                                } 
							?>
						</td>
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
											apcesfera as codigo, 
											CASE 
												WHEN apcesfera = 'M'  THEN 'Municipal' 
												WHEN apcesfera = 'E' THEN 'Estadual' 
											END as descricao
										FROM 
											projovemcampo.adesaoprojovemcampo";
								
								$db->monta_combo('apcesfera', $sql, $bloq, 'Selecione', '', '', '', '', 'S', 'apcesfera'); 
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
                    <tr id="tr_escola">
                        <td class="SubTituloDireita" width="15%">Transferir para o Escola:</td>
                        <td id="td_escola" >
                            <? 
							if($entidDestino!=''){
								buscarEscolaDestino();
							}else{
								$db->monta_combo('entidDestino', array(), 'S', 'Selecione um ano de execução', '', '', '', '', 'S', 'entidDestino');
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

$where = array("hst.shtid_status in(1,4)");

if(!$db->testa_superuser()) {
    $perfis = pegaPerfilGeral();
//     $where[] = "tra.usucpfresponsavelorigem = '{$_SESSION['usucpf']}'";
//     $where[] = "tra.aprid_origem = {$_SESSION['projovemcampo']['aprid']}";
}

if( in_array(PFL_COORDENADOR_ESTADUAL, $perfis) ){
    $inner_local = "INNER JOIN territorios.estado e ON e.estuf = pro.estuf";
}elseif(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
    $inner_local = "INNER JOIN territorios.municipio m ON m.muncod = pro.muncod";
}
    
// // ver($_SESSION['projovemcampo']['pjuid'],d);
if($_SESSION['projovemcampo']['modalidade'] == 'M'){   
    
    $where[] = " tra.turid_destino IS NOT NULL ";
    $sql = "SELECT DISTINCT
				'<center><input type= \"checkbox\" class=\"doc\" name=\"traid[]\" value=\"'|| tra.traid ||'\" /></center>' as marcar,
                '<center><img src=../imagens/excluir.gif border=0 width=13px
                      title=Excluir style=cursor:pointer; 
                      id='|| tra.traid ||' class=excluir /></center>' as excluir,
				est.estcpf as cpf,
				est.estnome as nome,
				CASE 
					WHEN apcO.apcesfera = 'E'
					THEN munO.estuf 
					ELSE munO.estuf||' - '||munO.mundescricao
				END as localOrigem,
				--'Escola'||' '||
				entD.entnome as escolaOrigem,
				turO.turdescricao as turmaOrigem,
				aprO.apranoreferencia as anoOrigem,
				CASE 
					WHEN apcD.apcesfera = 'E'
					THEN munD.estuf 
					ELSE munD.estuf||' - '||munD.mundescricao
				END as localDestino,
				--'Escola'||' '||
				entD.entnome as escolaDestino,
				turD.turdescricao as turmaDestino,
				aprD.apranoreferencia as anoDestino,
				CASE 
					WHEN arqt.arqid IS NULL
					THEN 'Não Existente'
					ELSE '<center><a id='||arqt.arqid||' style=\"cursor:pointer;color:blue\" class=baixar>Baixar</a></center>'
				END as arquivo
			FROM
				projovemcampo.transferencia tra
			INNER JOIN projovemcampo.estudante 	est ON est.estid = tra.estid
			INNER JOIN projovemcampo.historico_transferencia hst ON hst.htrid = tra.htrid_ultimo
			LEFT JOIN projovemcampo.arquivo_tranferencia arqt ON arqt.traid = tra.traid
			-- Dados Origem
			INNER JOIN projovemcampo.turma turO ON turO.turid = tra.turid_origem
			INNER JOIN projovemcampo.adesaoprojovemcampo apcO ON apcO.secaid = turO.secaid
			INNER JOIN projovemcampo.anoprograma aprO ON aprO.aprid = apcO.aprid
			INNER JOIN entidade.entidade entO ON entO.entid = turO.entid
			INNER JOIN entidade.endereco endeO ON endeO.entid = entO.entid
			INNER JOIN territorios.municipio munO ON munO.muncod = endeO.muncod
			--Dados Destino
			INNER JOIN projovemcampo.turma turD ON turD.turid = tra.turid_destino
			INNER JOIN projovemcampo.adesaoprojovemcampo apcD ON apcD.secaid = turD.secaid
			INNER JOIN projovemcampo.anoprograma aprD ON aprD.aprid = apcD.aprid
			INNER JOIN entidade.entidade entD ON entD.entid = turD.entid
			INNER JOIN entidade.endereco endeD ON endeD.entid = entD.entid
			INNER JOIN territorios.municipio munD ON munD.muncod = endeD.muncod
			WHERE
				apcO.apcid = {$_SESSION['projovemcampo']['apcid']} 
			AND  ".implode(' AND ', $where)." 
            ORDER BY
                nome";	
//    ver($sql,d);
        $cabecalho = array($check,"Excluir","CPF Aluno","Aluno","Local Origem","Escola Origem","Turma Origem","Ano Origem","Local Destino","Escola Destino","Turma Destino","Ano destino","Arquivo Anexo");
                
 }elseif($_SESSION['projovemcampo']['modalidade'] == 'O'){
    
    $where[] = " tra.turid_destino IS NULL ";
    
  	$sql = "SELECT DISTINCT
				'<center><input type=\"checkbox\" class=\"doc\" name=\"traid[]\" value=\"'|| tra.traid ||'\" /></center>' as marcar,
                '<center><img src=../imagens/excluir.gif border=0 width=13px
                      title=Excluir style=cursor:pointer; 
                      id='|| tra.traid ||' class=excluir /></center>' as excluir,
				est.estcpf as cpf,
				est.estnome as nome,
				CASE 
					WHEN apcO.apcesfera = 'E'
					THEN munO.estuf 
					ELSE munO.estuf||' - '||munO.mundescricao
				END as localOrigem,
				--'Escola'||' '||
				entD.entnome as escolaOrigem,
				turO.turdescricao as turmaOrigem,
				aprO.apranoreferencia as anoOrigem,
				CASE 
					WHEN apcD.apcesfera = 'E'
					THEN munD.estuf 
					ELSE munD.estuf||' - '||munD.mundescricao
				END as localDestino,
				--'Escola'||' '||
				entD.entnome as escolaDestino,
				aprD.apranoreferencia as anoDestino,
				CASE 
					WHEN arqt.arqid IS NULL
					THEN 'Não Existente'
					ELSE '<center><a id='||arqt.arqid||' style=\"cursor:pointer;color:blue\" class=baixar>Baixar</a></center>'
				END as arquivo
			FROM
				projovemcampo.transferencia tra
			INNER JOIN projovemcampo.estudante 	est ON est.estid = tra.estid
			INNER JOIN projovemcampo.historico_transferencia hst ON hst.htrid = tra.htrid_ultimo
			LEFT JOIN projovemcampo.arquivo_tranferencia arqt ON arqt.traid = tra.traid
			-- Dados Origem
			INNER JOIN projovemcampo.turma turO ON turO.turid = tra.turid_origem
			INNER JOIN projovemcampo.adesaoprojovemcampo apcO ON apcO.secaid = turO.secaid
			INNER JOIN projovemcampo.anoprograma aprO ON aprO.aprid = apcO.aprid
			INNER JOIN entidade.entidade entO ON entO.entid = turO.entid
			INNER JOIN entidade.endereco endeO ON endeO.entid = entO.entid
			INNER JOIN territorios.municipio munO ON munO.muncod = endeO.muncod
			--Dados Destino
			INNER JOIN entidade.entidade entD ON entD.entid = tra.entid_destino
			INNER JOIN projovemcampo.turma turD ON turD.entid = entD.entid
			INNER JOIN projovemcampo.adesaoprojovemcampo apcD ON apcD.secaid = turD.secaid
			INNER JOIN entidade.endereco endeD ON endeD.entid = entD.entid
			INNER JOIN projovemcampo.anoprograma aprD ON aprD.aprid = apcD.aprid
			INNER JOIN territorios.municipio munD ON munD.muncod = endeD.muncod
			WHERE 
            	apcO.apcid = {$_SESSION['projovemcampo']['apcid']} 
 			AND ".implode(' AND ', $where)." 
            ORDER BY
                nome";
//   	ver($sql);
    $cabecalho = array($check,"Excluir","CPF Aluno","Aluno","Local Origem","Escola Origem","Turma Origem","Ano Origem","Local Destino","Escola Destino","Ano destino","Arquivo Anexo");
 }	
    echo "<form name=form_transferencias id=form_transferencias method=POST>";
    $db->monta_lista_simples($sql, $cabecalho, 100, 5, 'CENTER', '95%', '', '');
    echo "</form>";
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="Center" border="0">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">
		 <?if(!in_array ( PFL_CONSULTA, $perfis )){?>
		  <input type="button" class="transferir" value="Transferir">
		  <?php }}?>	
		</td>
	</tr>
</table>
<?
//}
?>
<? 
//registarUltimoAcesso(); 
?>
