<?php
$permissao_formulario = 'S';

$acoid          = $_REQUEST['acoid'];
$acompanhamento = new Acompanhamento($acoid);
if($acoid){
    extract($acompanhamento->getDados());
}

$modelo   = new ContratacaoFase();
$arEtapas = $modelo->getContratacaoFaseByContratacao($_REQUEST['conid']);


if ($_POST['act'] == 'salvar'){
    if($_REQUEST['acoid'] == ''){
        $_REQUEST['usucpf']          = $_SESSION['usucpf'];
        $_REQUEST['acostatus']       = 'A';
        $_REQUEST['acodatacadastro'] = 'now()';
    }

    $acoid = $acompanhamento->popularDadosObjeto()
                            ->salvar();


    $acompfase = new AcompanhamentoFase();
    $acompfase->deleteByAcompanhamento($acoid);

    if(is_array($_REQUEST['stfid']) && !empty($_REQUEST['stfid'])){
	    foreach ( $_REQUEST['stfid'] as $cofid => $value ){
		    $arDados['acoid']             = $acoid;
            $arDados['cofid']             = $cofid;
            $arDados['stfid']             = $value;

		    $arCamposNulos = array();
		    if($_REQUEST['acfdataconclusao'][$cofid] == ''){
		        $arDados['acfdataconclusao'] = null;
		        $arCamposNulos[]             = 'acfdataconclusao';
		    }else{
		        $arDados['acfdataconclusao'] = formata_data_sql($_REQUEST['acfdataconclusao'][$cofid]);
		    }

            $acompfase->popularDadosObjeto($arDados)->salvar(true, true, $arCamposNulos);
            $acompfase->setDadosNull();
	    }
    }

    $acompanhamento->commit();
    $db->sucesso( 'principal/listarAcompanhamento', "&acoid={$acoid}&atiid={$_REQUEST['atiid']}&conid={$_REQUEST['conid']}");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

if( $_REQUEST['conid'] && $_REQUEST['atiid']){
    $db->cria_aba($abacod_tela, $url, "&atiid={$_REQUEST['atiid']}&conid={$_REQUEST['conid']}");
}

include APPRAIZ . 'sisplan/modulos/principal/cabecalhopa.inc';

monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" rel="stylesheet"></link>
<form method="POST" name="formulario" action="">
    <input type="hidden" name="acoid" value="<?= $acoid; ?>"/>
    <input type=hidden   name="act"   value="0"/>

    <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita">
                Situação do processo de contratação:
            </td>
            <td><?php
                $sql = "SELECT stpid AS codigo, stpdsc AS descricao FROM sisplan.situacaoprocesso WHERE stpstatus = 'A' ORDER BY 2";
                $db->monta_combo("stpid",$sql,$permissao_formulario,'&nbsp','','','','','','stpid');
                echo  obrigatorio();
            ?></td>
        </tr>
        <tr>
            <td colspan="2" class="SubTituloEsquerda" align="center" style="text-align:center;">Etapas previstas</td>
        </tr>
        <tr>
	        <td colspan="2">
	            <br>
	            <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width:70%;">
	               <tr>
	                   <td class="SubTituloEsquerda" align="center" style="width:50%;">Etapa</td>
	                   <td class="SubTituloEsquerda" align="center" style="width:25%;"><center>Situação</td>
	                   <td class="SubTituloEsquerda" align="center" style="width:25%;"><center>Data de Conclusão</center></td>
	               </tr>
        <?php
              if( is_array($arEtapas) && !empty($arEtapas)){

              	  $UltimoAcompanhamento = $acompanhamento->getUltimoAcompanhamento($_REQUEST['conid']) ;

              	  foreach($arEtapas  as $chave => $etapa){

	              	   if($_REQUEST['acoid']){
	                       $modelo                  = new AcompanhamentoFase( $_REQUEST['acoid'], $etapa['cofid'] );
	                       $dadosAcompanhamentoFase = $modelo->getDados();
	              	   }else{
	                       $modelo                  = new AcompanhamentoFase( $UltimoAcompanhamento['acoid'], $etapa['cofid'] );
	                       $dadosAcompanhamentoFase = $modelo->getDados();
	              	   }
	    ?>
		              <tr bgcolor="<?php echo ($chave % 2 == 0) ? '#FFFFFF' : '#F7F7F7' ?>">
		                 <td> <?php echo $etapa['facnome']; ?> </td>
		                 <td align="Center">
		                        <div>
		                            <?php
		                                $modelo = new SituacaoFase();
		                                $dados  = $modelo->recuperarTodos("stfid as codigo, stfnome as descricao", array("stfstatus = 'A'"), "stfordem");

		                                $db->monta_combo("stfid[{$etapa['cofid']}]",$dados,$permissao_formulario,'&nbsp','exibeData(this);','','','','',"stfid_{$etapa['cofid']}",false,$dadosAcompanhamentoFase['stfid']);
						                echo  obrigatorio();

						                if($dadosAcompanhamentoFase['stfid'] == SITUACAO_FASE_CONCLUIDA){
						                   $display = '';
						                }else{
						                   $display = 'display:none;';
						                }
		                            ?>
		                        </div>
		                 </td>
		                 <td  align="Center">
	                         <div id="divData_<?php echo $etapa['cofid']; ?>" style="<?php echo $display; ?>">
						         <?
						             echo campo_data2( "acfdataconclusao[{$etapa['cofid']}]", 'S', $permissao_formulario, '', 'S','','',$dadosAcompanhamentoFase['acfdataconclusao'],'','',"acfdataconclusao_{$etapa['cofid']}" ); ?>
	                         </div>
		                 </td>
		              </tr>
        <?php    }
              }else{ ?>
                  <tr>
                     <td colspan="2" align="Center">
                        Nenhuma etapa cadastrada
                     </td>
                  </tr>
        <?php } ?>
         </table>
         <br>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="SubTituloEsquerda" align="center" style="text-align:center;"></td>
        </tr>
        <tr>
           <td class="SubTituloDireita">Observações</td>
           <td>
               <?= campo_textarea( 'acoobservacoes', 'S', $permissao_formulario, '', 100, 8, 500,'','',''); ?>
           </td>
        </tr>
        <tr style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%"> </td>
            <td>
                <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
                <input type="button" name="voltar" value="Voltar" onclick="javascript:location.href='?modulo=principal/listarAcompanhamento&acao=A&atiid=<?= $_REQUEST['atiid']; ?>&conid=<?= $_REQUEST['conid']; ?>'"/>
            </td>
        </tr>
    </table>
</form>

<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function exibeData(elemento){
	var elemId = elemento.id.split("_");

	if( elemento.value == '<?php echo SITUACAO_FASE_CONCLUIDA; ?>' ){
		$('#divData_'+elemId[1]).show();
	}else {
		$('#divData_'+elemId[1]).hide();
		$('#acfdataconclusao_'+elemId[1]).val('');
	}
}
function validar_cadastro(cod){
	var retorno = true;

    if (!validaBranco(document.formulario.stpid, 'Situação do processo de contratação')) return;

    $('[id^=stfid_]').each(function(index, elem){
        if($(elem).val() == ''){
            alert('Todas as situações das Etapas Previstas devem ser preenchidas.');
            retorno = false;
            return retorno;
        }else{
            if( elem.value == '<?php echo SITUACAO_FASE_CONCLUIDA; ?>'){
            	var elemId = elem.id.split("_");
            	if( $('#acfdataconclusao_'+elemId[1]).val() == '' ){
                    alert('Todas as datas de conclusão das Etapas Previstas devem ser preenchidas.');
                    retorno = false;
                    return retorno;
            	}else{
            		var dtFim = new String($('#acfdataconclusao_'+elemId[1]).val()).split('/');
                    dtFim     = new Date(dtFim[2], new Number(dtFim[1])-1, dtFim[0]);
                	if( dtFim > new Date() ){
                        alert('Nenhuma data de conclusão das Etapas Previstas pode ser maiores que a data de hoje.');
                        retorno = false;
                        return retorno;
                	}


            	}
            }
        }
    });

    if(retorno == false){
        return retorno;
    }

    if (!validaBranco(document.formulario.acoobservacoes, 'Observações')) return;


    document.formulario.act.value = 'salvar';

    document.formulario.submit();
}


</script>