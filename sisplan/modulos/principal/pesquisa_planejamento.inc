<?php
$planTatico = new PlanejamentoTatico();
extract($_REQUEST);

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
        die();
    }elseif ( $_REQUEST['act'] == 'remover' && $pltid){

        $planTaticoExcluir = new PlanejamentoTatico($pltid);
        $planTaticoExcluir->popularDadosObjeto(array('pltstatus' => 'I'))->salvar();
        $planTaticoExcluir->commit();
        $db->sucesso($_REQUEST['modulo']);

    }
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( $titulo_modulo, '<img title="Alterar Planejamento" src="/imagens/alterar.gif ">&nbsp; Alterar Planejamento &nbsp;
                               <img title="Excluir Planejamento" src="/imagens/excluir.gif ">&nbsp; Excluir Planejamento &nbsp;
                               <img title="Criar PA a partir do planejamento" src="/imagens/gif_inclui.gif">&nbsp; Criar PA a partir do Planejamento &nbsp;
                               <img title="Ir para PA deste planejamento" src="/imagens/gif_inclui_d.gif">&nbsp; Ir para PA do Planejamento &nbsp;
                               <img title="Visualizar PA do Planejamento" src="/imagens/consultar.gif">&nbsp; Visualizar PA do Planejamento
                               ');
$permissao_formulario = 'S';
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript">

    function filtraMunicipio(estuf){
        $('#muncodTd').html('carregando...');
        $('#muncodTd').load('?modulo=principal/inclusao_planejamento&acao=C',
                          {'act'  : 'ajax',
                           'estuf': estuf});
    }

	function pesquisar()
	{
		document.formulario.act.value = 'pesquisar';
		document.formulario.submit();
	}

	function limpar()
	{
		document.formulario.act.value     = 'pesquisar';
		document.formulario.pltid.value   = '';
		document.formulario.prgcod.value  = '';
		document.formulario.acacod.value  = '';
		document.formulario.pltnome.value = '';
		document.formulario.stpid.value   = '';
		document.formulario.estuf.value   = '';
		filtraMunicipio('');
		document.formulario.uexid.value   = '';
		document.getElementById('vinculadoTodos').checked = 'checked';
	}

	function alterar( pltid )
	{
		location.href = '<?=$_SESSION['sisdiretorio']?>.php?modulo=principal/inclusao_planejamento&acao=C&pltid='+pltid;
	}

	function excluir( pltid )
	{
		if ( confirm( 'Deseja excluir esse item' ) ) {
			document.formulario.pltid.value = pltid;
			document.formulario.act.value = 'remover';
			document.formulario.submit();
		}
	}

	function criarPa(pltid){
	    window.location = '?modulo=principal/cadpa&acao=I&pltid='+pltid;
	}

	function irParaPa(atiid){
	    window.location = '?modulo=principal/cadpa&acao=I&atiid='+atiid;
	}

	function visualizarPa(atiid){
	    var w = window.open(
	    		'?modulo=principal/dadospa&acao=C&atiid='+atiid,
	            'dadospa',
	            'width=670,height=580,scrollbars=yes,scrolling=yes,resizebled=yes'
	            );
	    w.focus();
	}

    function filtraAcao(prgcod){
        $('#acaoTd').html('carregando...');
        $('#acaoTd').load('?modulo=principal/inclusao_planejamento&acao=C',
                          {'act'    : 'ajax',
                           'prgcod' : prgcod});
    }

</script>

<form method="POST" name="formulario">
	<input type=hidden name="act" value="pesquisar"/>
	<input type=hidden name="pltid" value="0"/>

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
                <?= campo_texto('pltnome', 'N', $permissao_formulario, '', 80, 500, '', ''); ?>
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
                    $db->monta_combo("stpid", $sql, $permissao_formulario, '&nbsp', '', '', '', '580', 'N', 'stpid');
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
                    $db->monta_combo("uexid", $sql, $permissao_formulario, '&nbsp', '', '', '', '580', 'N', 'uexid');
                ?>
            </td>
        </tr>
        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">
                 Plano de Ação Vinculado:
            </td>
            <td>
                <input type="radio" id="vinculadoSim" name="vinculado" value="sim"   <?= $vinculado == 'sim' ? 'checked="checked"' : '' ?>> Sim
                <input type="radio" id="vinculadoNao" name="vinculado" value="nao"   <?= $vinculado == 'nao' ? 'checked="checked"' : '' ?>> Não
                <input type="radio" id="vinculadoTodos" name="vinculado" value="todos" <?= $vinculado == 'todos' || is_null($vinculado) ? 'checked="checked"' : '' ?>> Todos

            </td>
        </tr>
        <tr style="background-color: #cccccc">
            <td align="right" style="vertical-align:top; width:25%;">
                &nbsp;
            </td>
            <td colspan="">
                <input type="button" name="btnPesquisar" value="Pesquisar" onclick="pesquisar();"/>
                <input type="button" name="btnLimpar" value="Limpar" onclick="limpar();"/>
            </td>
        </tr>
    </table>
</form>

<?php
$where = array();

if ( !empty($prgcod) )
    $where[] = "plt.prgcod = '{$prgcod}' ";

if ( !empty($acacod) )
    $where[] = "plt.acacod = '{$acacod}' ";

if ( !empty($pltnome) )
	$where[] = "plt.pltnome ILIKE '%{$pltnome}%' ";

if ( !empty($stpid) )
    $where[] = "plt.stpid = '{$stpid}' ";

if ( !empty($estuf) )
    $where[] = "plt.estuf = '{$estuf}' ";

if ( !empty($muncod) )
    $where[] = "plt.muncod = '{$muncod}' ";

if ( !empty($uexid) )
    $where[] = "plt.uexid = '{$uexid}' ";

if ( !empty($vinculado) )
    $where['vinculado'] = $vinculado;

$dados  = $planTatico->listarByWhere($where);

// CABEÇALHO da lista
$arCabecalho = array('<center>Ações</center>',
					 'Nome',
					 'Programa',
					 'Ação',
					 'Prioridade',
					 'Situação',
					 'UF',
					 'Município',
					 'Unidade Executora',
					 'Custeio',
					 'Investimento');

$acao  = '<center>';
$acao .= "<img onclick=\"alterar('{pltid}')\" src=\"/imagens/alterar.gif\" style=\"border:none;cursor:pointer;\" title=\"Alterar item\">";
$acao .= "&nbsp;";
$acao .= "<img onclick=\"excluir('{pltid}')\" src=\"/imagens/excluir.gif\" style=\"border:none;cursor:pointer;\" title=\"Excluir item\">";
$acao .= "&nbsp;";
$acao .= '<php>
            GLOBAL $db;
            $sql = "SELECT atiid FROM pde.atividade WHERE pltid = {pltid}";
            $atiid = $db->pegaUm($sql);
            if( $atiid ){
                return "<img onclick=\"irParaPa(\'$atiid\')\" src=\"/imagens/gif_inclui_d.gif\" style=\"border:none;cursor:pointer;\" title=\"Ir para PA deste Planejamento\">&nbsp;
                        <img onclick=\"visualizarPa(\'$atiid\')\" src=\"/imagens/consultar.gif\" style=\"border:none;cursor:pointer;\" title=\"Visualizar PA deste Planejamento\">";
            }else{
                return "<img onclick=\"criarPa(\'{pltid}\')\" src=\"/imagens/gif_inclui.gif\" style=\"border:none;cursor:pointer;\" title=\"Criar PA a partir deste Planejamento\">";
            }
          </php>
         ';
$acao .= '</center>';

// ARRAY de parametros de configuração da tabela
$arConfig = array("style"         => "width:95%;",
				  "totalLinha"    => false,
				  "totalRegistro" => true);

$arParamCol[0]['style'] = 'width:120px;';

//$arParamCol[2]['style'] = 'width:120px;';
//$arParamCol[3]['style'] = 'width:70px;';
//$arParamCol[4]['style'] = 'width:150px;';

$oLista = new Lista($arConfig);
$oLista->setCabecalho( $arCabecalho );
$oLista->setCorpo( $dados, $arParamCol );
$oLista->setAcao( $acao );
$oLista->show();
?>