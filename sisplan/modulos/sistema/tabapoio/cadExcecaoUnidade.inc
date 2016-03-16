<?php

//Verificando a data de inicio e termino do planejamento ( definida na programacaoexercicio )
$programacaoExercicio = new ProgramacaoExercicio();
$permissao = $programacaoExercicio->verificaPermissaoPlanejamento();

$permissao_formulario = 'S';
$id     = $_REQUEST['exuid'];
$modelo = new ExcecaoUnidade($id);
extract($modelo->getDados());

if ($_POST['act'] == 'salvar'){
    if(is_array($_REQUEST['uexid'])){
        foreach ($_REQUEST['uexid'] as $uexid){
            $arDados = array(
                                'exuid'     => $_REQUEST['exuid'],
                                'exudata'   => formata_data_sql($_REQUEST['exudata']),
                                'uexid'     => $uexid,
                                'prsano'    => $_SESSION['exercicio'],
                                'exustatus' => 'A'
                              );
            $modelo->popularDadosObjeto($arDados)->salvar();
            $modelo->setDadosNull();
        }
    }else{
        $arDados = array(
                            'exuid'     => $_REQUEST['exuid'],
                            'exudata'   => formata_data_sql($_REQUEST['exudata']),
                            'exustatus' => 'A'
                          );
        $modelo->popularDadosObjeto($arDados)->salvar();
    }
    $modelo->commit();

    $db->sucesso( 'sistema/tabapoio/excecaoUnidade', '');
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$titulo = (empty($id) ? 'Cadastro de Exceção Unidade' : 'Alteração de  Exceção Unidade');
monta_titulo( $titulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form method="POST" name="formulario" action="">
<input type="hidden" name="exuid" value="<?= $exuid; ?>"/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
     <tr>
        <td align='right' class="SubTituloDireita" style="width:25%;">
             Unidade:
        </td>
        <td>
            <?php

				$where = array();
            	if( !$db->testa_superuser() ){
					if(is_array( $responsabilidades ) && !empty( $responsabilidades )){
						$where[] = " uex.uexid IN ('".implode("','", $responsabilidades)."') ";
					}else{
						$where[] = " FALSE ";
					}
				}else{
					$where[] = " TRUE ";
				}

                $modelo = new UnidadeExecutora();
                $sql = $modelo->getSqlComboFiltradoResponsabilidade($where);

                if($uexid){
	                $db->monta_combo("uexid", $sql, 'N', '&nbsp', '', '', '', '440', 'S', 'uexid');
                }else{

                    combo_popup(
						'uexid',
						$sql,
						'Selecione a(s) Unidades(s) Executoras(s)',
						'400x400',
						0,
						array(),
						$permissao_formulario,
						$permissao_formulario,
						true
					);

                    echo obrigatorio();
                }
            ?>
        </td>
    </tr>
     <tr>
        <td align='right' class="SubTituloDireita">
            Data:
        </td>
        <td>
            <?=  campo_data2('exudata', 'S', 'S', '', 'S'); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%">
            &nbsp;
        </td>
        <td>
            <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
            <input type="button" name="Voltar" value="Voltar" onclick="javascript:location.href='?modulo=sistema/tabapoio/excecaoUnidade&acao=A'"/>
        </td>
    </tr>
</table>
</form>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >


function validar_cadastro(){

	<?php if($exsid == '') {?>
	        selectAllOptions( document.getElementById('uexid') );
	        if (!validaBranco(document.getElementById('uexid') , 'Unidade')) return;
	<?php } ?>

    if (!validaBranco(document.formulario.exudata, 'Data')) return;

    document.formulario.act.value = 'salvar';
    document.formulario.submit();
}

</script>

