
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="./js/gestaodocumentos.js"></script>
<script type="text/javascript" src="./js/ajax.js"></script>
<script type="text/javascript" src="../includes/wz_tooltip.js"></script>

<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	type="text/javascript" src="../includes/blendtrans.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>
<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/slider/slider.css"></link>

<!-- LINKS MODAL -->
<link type="text/css" rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" media="screen"/>

<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>

<!-- (FIM) BIBLIOTECAS -->
<!-- (INÍCIO) BLOCOS INTERNOS DE HTML - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<div id="sliderDiv"	style="z-index: 1000; width: 200px; *height: 97px; left: 2px; top: 2px; visibility: visible; display: none;">
    <div class="monthYearPicker" style="left: 167px; width: 35px;" id="minuteDropDown"></div>
    <div class="topBar" id="topBar" style="top: 150px;">
        <img onclick="removeSlider()" onmouseover="this.src = '../includes/JsLibrary/slider/images/close_over.gif'" onmouseout="this.src = '../includes/JsLibrary/slider/images/close.gif'" src="../includes/JsLibrary/slider/images/close.gif" style="position: absolute; right: 2px;" />
    </div>
    <div>
        <table cellspacing="1" width="100%">
            <tr>
                <td style="background-color: #E2EBED">Situação:</td>
                <td style="text-align:left">
                    <select onchange="alteraStatus(this)" id="situacaoSlider" style="border:1px; border-style:solid; border-color:black; width:100%; font-size:10px;">
                        <option value='1'>Não Iniciada</option>
                        <option value='2'>Em Andamento</option>
                        <option value='3'>Suspensa</option>
                        <option value='4'>Cancelada</option>
                        <option value='5'>Concluída</option>
                    </select>
                </td>
            </tr>
            <tr>
                <td style="background-color: #E2EBED">Andamento:</td>
                <td>
                    <span id="slider_target" style="width: 40px; position: relative; *left: -41px; top: 5px; height: 3px;"></span>&nbsp; 
                    <input type="text" name="valor" id="valorSlider" value="200" style="font-size: 10px; width: 30px" readonly />%</td>
            <script>
                form_widget_amount_slider('slider_target',document.getElementById( "valorSlider" ),70,0,100,"arredonda(document.getElementById( 'valorSlider' ))");
            </script>
            </tr>
            <tr>
                <td colspan="2" style="background-color: #E2EBED">
                    <input type="button" onclick="slicerSubmitSituacao()" value="Ok" style="height:18px; font-size: 10px;"/>
                </td>
            </tr>
        </table>
    </div>
</div>

<div id="comboDiv" style="z-index: 1000; width: 200px; *height: 97px; left: 2px; top: 2px; display: none;">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        <tr bgcolor="#708090">
            <td align="right">
                <img src="../imagens/excluir_2.gif" onclick="removeComboResponsavel();" style="cursor: pointer;" width="18px" height="18px" border="0" align="middle">
            </td>
        </tr>
        <tr>
            <td><b>Pessoa Responsável:</b></td>
        </tr>
        <tr>
            <td id="td_usucpfresponsavel">
            <?php 
                $db->monta_combo('usucpfresponsavelArvore', array(), 'S', "Selecione...", 'aposAlterarResponsavel', '', '', '200', 'N', 'usucpfresponsavelArvore',false,null,'Responsável pela Tarefa');
            ?> 
            </td>
        </tr>
    </table>
        <input type="hidden" id="idComboResp" value="200" style="font-size: 10px; width: 30px" readonly />
</div>

<input type="hidden" id="inputGeral" value="" readonly="readonly" onchange="desmontaCalendario(this)"/>
<!-- (FIM) BLOCOS INTERNOS DE HTML -->

<div id="teste"></div>
<center>
	<div id="aguarde_" style="display: none;position:absolute;color:#000033;top:50%;left:35%; width:300;font-size:12px;z-index:0;">
		<br><img src="../imagens/carregando.gif" border="0" align="middle"><br>Carregando...<br>
	</div>
</center>

<script type="text/javascript">
    /*
    function recuperaSetorUsuarioLogado(){
        return '<?php echo $_SESSION['gt_doc']['setorUsuarioLogado']; ?>';
    }
    
    function recuperaCpfUsuarioLogado(){
        return '<?php echo $_SESSION['usucpf']; ?>';
    }
*/
    function verificaSePermitido(){
        return '<?php echo $_SESSION['gt_doc']['perfilHabilitado']; ?>';
    }

    function verificaSeSuperUsuario(){
        return '<?php echo $_SESSION['gt_doc']['perfilSuperUsuario']; ?>';
    }

</script>