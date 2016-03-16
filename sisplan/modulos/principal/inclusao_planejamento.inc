<?php
$permissao_formulario = 'S';
if($_REQUEST['act']){
    if($_REQUEST['act'] == 'ajax'){
        if(isset($_REQUEST['estuf'])){
            if( $_REQUEST['estuf'] != '' ){
                $sql = "SELECT
                            muncod AS codigo,
                            mundescricao AS descricao
                        FROM
                            territorios.municipio
                        WHERE
                            estuf = '{$_REQUEST['estuf']}'
                        ORDER BY
                            2";
                $habilitado = $permissao_formulario;
            }elseif($_REQUEST['estuf'] == ''){
                $sql = array(array('codigo' => 0, 'descricao' => ''));
                $habilitado = 'N';
            }
            $db->monta_combo("muncod", $sql, $habilitado, '&nbsp', '', '', '', '195', 'N', 'muncod');
        }elseif(isset($_REQUEST['prgcod'])){
            $prgcod = $_REQUEST['prgcod'];
             if($prgcod != ''){
                $sql   = "SELECT prgid FROM monitora.programa WHERE prgcod = '{$prgcod}' AND prgano = '{$_SESSION['exercicio']}'";
                $prgid = $db->pegaUm($sql);

                $sql = "SELECT DISTINCT
                            acacod as codigo,
                            acacod || ' - ' || acadsc AS descricao
                        FROM
                            monitora.acao a
                        WHERE
                            acasnrap      = false
                            AND prgano    = '{$_SESSION['exercicio']}'
                            AND acastatus = 'A'
                            AND prgcod    = '{$prgcod}'
                            AND prgid     = {$prgid}
                            AND prgano    = '{$_SESSION['exercicio']}'
                        ORDER BY
                            2";
                $acaoHabilitada = $permissao_formulario;
            }else{
                $sql = array(array('codigo' => '', 'descricao'=>'Selecione o Programa...'));
                $acaoHabilitada = 'N';
            }

            $db->monta_combo("acacod", $sql,$acaoHabilitada, '&nbsp', '', '', '', '580', 'S', 'acacod');
        }
    }elseif($_REQUEST['act'] == 'gravar'){

        if( $_REQUEST['pltid'] == '' ){
            $_REQUEST['usucpf'] = $_SESSION['usucpf'];
            $_REQUEST['pltano'] = $_SESSION['exercicio'];
        }

        $_REQUEST['pltinvestimento'] = str_replace(array('.',','), array('','.'), $_REQUEST['pltinvestimento']);
        $_REQUEST['pltcusteio']      = str_replace(array('.',','), array('','.'), $_REQUEST['pltcusteio']);

        $planTatico = new PlanejamentoTatico();

        try{
            $pltid = $planTatico->popularDadosObjeto()->salvar();
            $planTatico->commit();

            $_REQUEST['acao'] = 'C';
            $db->sucesso( 'principal/pesquisa_planejamento', '' );
        }catch ( Exception $e ){
            $planTatico->rollback();
            die('<script>alert(\'Não foi possível realizar a operação!\'); history.go(-1);</script>');
        }
    }
    die();
}

if( $_REQUEST['pltid'] ){
    try{
        $planTatico = new PlanejamentoTatico($_REQUEST['pltid']);
        extract($planTatico->getDados());
        $pltcusteio      = formata_valor($pltcusteio);
        $pltinvestimento = formata_valor($pltinvestimento);
        $titulo_modulo = 'Alterar Planejamento';
    }catch ( Exception $e ){
        //não busca...
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>
<form action="" method="post" name="formulario">
	<input type="hidden" name="pltid" value="<?= $pltid; ?>"/>
	<input type="hidden" name="act" value="gravar"/>
    <table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Programa:
		    </td>
            <td>
                <?php
                    $sql = "SELECT DISTINCT
                                p.prgcod AS codigo,
                                p.prgcod || ' - ' || p.prgdsc AS descricao
                            FROM
                                monitora.programa p
                            WHERE
                                p.prgano = '{$_SESSION['exercicio']}'
                                AND prgstatus = 'A'
                            ORDER BY
                                2";
                    $db->monta_combo("prgcod", $sql, $permissao_formulario, '&nbsp', 'filtraAcao', '', '', '580', 'S', 'prgcod');
                ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Ação:
		    </td>
            <td id="acaoTd">
                <?php
                    if($prgcod){
                        $sql   = "SELECT prgid FROM monitora.programa WHERE prgcod = '{$prgcod}' AND prgano = '{$_SESSION['exercicio']}'";
                        $prgid = $db->pegaUm($sql);

                        $sql = "SELECT DISTINCT
                                    acacod as codigo,
                                    acacod || ' - ' || acadsc AS descricao
                                FROM
                                    monitora.acao a
                                WHERE
                                    acasnrap      = false
                                    AND prgano    = '{$_SESSION['exercicio']}'
                                    AND acastatus = 'A'
                                    AND prgcod    = '{$prgcod}'
                                    AND prgid     = {$prgid}
                                    AND prgano    = '{$_SESSION['exercicio']}'
                                ORDER BY
                                    2";
                        $acaoHabilitada = $permissao_formulario;
                    }else{
                        $sql = array(array('codigo' => '', 'descricao'=>'Selecione o Programa...'));
                        $acaoHabilitada = 'N';
                    }

                    $db->monta_combo("acacod", $sql,$acaoHabilitada, '&nbsp', '', '', '', '580', 'S', 'acacod');
                ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Nome:
		    </td>
            <td>
                <?= campo_textarea( 'pltnome', 'S', $permissao_formulario, '', 107, 3, 500 ); ?>
            </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
			     Prioridade:
		    </td>
			<td>
			     <?= campo_texto('pltprioridade', 'S', $permissao_formulario, '', 4, 4, '####', ''); ?>
		     </td>
		</tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Situação:
            </td>
            <td>
                <?php
                    $sql = "SELECT
                                stpid AS codigo,
                                stpdsc AS descricao
                            FROM
                                sisplan.situacaoprocesso
                            WHERE
                                stpstatus = 'A'
                            ORDER BY
                                2";
                    $db->monta_combo("stpid", $sql, $permissao_formulario, '&nbsp', '', '', '', '580', 'S', 'stpid');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 UF:
            </td>
            <td>
                <?php
                    $estado = new Estado();
                    $dados  = $estado->recuperarTodos("estuf as codigo, estuf as descricao", null, "descricao");
                    $db->monta_combo("estuf", $dados, $permissao_formulario, '&nbsp', 'filtraMunicipio', '', '', '', 'N', 'estuf');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Município:
            </td>
            <td id='muncodTd'>
                <?php
                    if($estuf){
                        $sql = "SELECT
                                    muncod AS codigo,
                                    mundescricao AS descricao
                                FROM
                                    territorios.municipio
                                WHERE
                                    estuf = '{$estuf}'
                                ORDER BY
                                    2";
                        $habilitado = $permissao_formulario;
                    }else{
                        $sql = array(array('codigo' => 0, 'descricao' => ''));
                        $habilitado = 'N';
                    }
                    $db->monta_combo("muncod", $sql, $habilitado, '&nbsp', '', '', '', '195', 'N', 'muncod');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Unidade Executora:
            </td>
            <td>
                <?php
                    $sql = "SELECT
                                uexid AS codigo,
                                uexsigla || ' - ' || uexdsc AS descricao
                            FROM
                                planointerno.unidadeexecutora
                            WHERE
                                uexstatus = 'A'
                            ORDER BY
                                2";
                    $db->monta_combo("uexid", $sql, $permissao_formulario, '&nbsp', '', '', '', '580', 'S', 'uexid');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Custeio:
            </td>
            <td>
                 <?= campo_texto('pltcusteio', 'S', $permissao_formulario, '', 25, 20, '#.###.###.###.###,##', ''); ?>
             </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Investimento:
            </td>
            <td>
                 <?= campo_texto('pltinvestimento', 'S', $permissao_formulario, '', 25, 20, '#.###.###.###.###,##', ''); ?>
             </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Observação:
            </td>
            <td>
                <?= campo_textarea( 'pltobservacao', 'N', $permissao_formulario, '', 107, 5, 1000 ); ?>
            </td>
        </tr>
        <tr style="background-color: #cccccc">
            <td align="right" style="vertical-align:top; width:25%;">
                &nbsp;
            </td>
            <td colspan="">
                <input type="button" name="btnGravar" value="Gravar" onclick="enviar();"/>
                <input type="button" name="btnVoltar" value="Cancelar" onclick="cancelar();"/>
            </td>
        </tr>
	</table>
</form>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

    function filtraMunicipio(estuf){
    	$('#muncodTd').html('carregando...');
        $('#muncodTd').load('?modulo=principal/inclusao_planejamento&acao=C',
        		          {'act'  : 'ajax',
                           'estuf': estuf});
    }

    function filtraAcao(prgcod){
    	$('#acaoTd').html('carregando...');
        $('#acaoTd').load('?modulo=principal/inclusao_planejamento&acao=C',
        		          {'act'    : 'ajax',
                           'prgcod' : prgcod});
    }

    function cancelar() {
		window.location.href = '?modulo=principal/pesquisa_planejamento&acao=C';
	}

	function enviar(){

		var d = document;
		var f = d.formulario;
		var msg = '';

		if (f.prgcod.value == ''){
			msg += '\n\tPrograma';
		}
		if (f.acacod.value == ''){
			msg += '\n\tAção';
		}
		if (f.pltnome.value == ''){
			msg += '\n\tNome';
		}
		if (f.pltprioridade.value == ''){
			msg += '\n\tPrioridade';
		}
		if (f.stpid.value == ''){
			msg += '\n\tSituação';
		}
		if (f.uexid.value == ''){
			msg += '\n\tUnidade Executora';
		}
		if (f.pltcusteio.value == ''){
			msg += '\n\tCusteio';
		}
		if (f.pltinvestimento.value == ''){
			msg += '\n\tInvestimento';
		}

		if (msg != ''){
			alert('Os campos listados são obrigatórios e devem ser preenchidos:' + msg);
			return false;
		}

		document.formulario.submit();
	}
</script>