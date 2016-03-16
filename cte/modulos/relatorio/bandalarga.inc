<?
if($_REQUEST['post']=='ok'){

    include 'bandalargaresultado.inc';
exit;    
}
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
cte_montaTitulo( $titulo_modulo, '&nbsp;' );
include APPRAIZ . 'includes/Agrupador.php';
?>
<script language="javascript" type="text/javascript" src="../includes/calendario.js">
<script type="text/javascript">

    function exibirRelatorio()
    {
        var formulario = document.filtro;
        
        // verifica se fisico e/ou financeiro esta selecionado
        var fisico = document.getElementById( 'fisico' ).checked;
        var financeiro = document.getElementById( 'financeiro' ).checked;
        if ( !fisico && !financeiro )
        {
            alert( 'Escolha ao menos um dos tipos "Físico" e "Financeiro".' );
            return;
        }
        
        // verifica se solicitado e/ou atendido esta selecionado
        var solicitado = document.getElementById( 'solicitado' ).checked;
        var atendido = document.getElementById( 'atendido' ).checked;
        if ( !solicitado && !atendido )
        {
            alert( 'Escolha ao menos um dos tipos "Solicitado" e "Atendido".' );
            return;
        }
        
        // verifica se municipal e/ou estadual esta selecionado
        var municipal = document.getElementById( 'municipal' ).checked;
        var estadual = document.getElementById( 'estadual' ).checked;
        if ( !municipal && !estadual )
        {
            alert( 'Escolha ao menos uma das demandas "Municipal" e "Estadual".' );
            return;
        }
        
        // verifica se tem algum agrupador selecionado
        agrupador = document.getElementById( 'agrupador' );
        if ( !agrupador.options.length )
        {
            alert( 'Escolha ao menos um item para agrupar o resultado.' );
            return;
        }
        
        selectAllOptions( agrupador );
        selectAllOptions( document.getElementById( 'regcod' ) );
        selectAllOptions( document.getElementById( 'estuf' ) );
        selectAllOptions( document.getElementById( 'dimid' ) );
        selectAllOptions( document.getElementById( 'ardid' ) );
        selectAllOptions( document.getElementById( 'indid' ) );
        selectAllOptions( document.getElementById( 'undid' ) );
        selectAllOptions( document.getElementById( 'prgid' ) );
        selectAllOptions( document.getElementById( 'aciparecer' ) );
        selectAllOptions( document.getElementById( 'psuid' ) );
        selectAllOptions( document.getElementById( 'gpsid' ) );
        selectAllOptions( document.getElementById( 'ssuid' ) );
        selectAllOptions( document.getElementById( 'frmid' ) );
        selectAllOptions( document.getElementById( 'foaid' ) );
        selectAllOptions( document.getElementById( 'labinstalado' ) );
        selectAllOptions( document.getElementById( 'BLInstal' ) );
        
        
        
        // submete formulario
        //alert( 'blz!' ); return;
        formulario.target = 'resultadoCteGeral';
        var janela = window.open( '', 'resultadoCteGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
        formulario.submit();
        janela.focus();
    }

</script>
<form action="" method="post" name="formulario">
<input type="hidden" name="post">

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita" valign="top" width="20%">Agrupadores:</td>
            <td width="80%">
                <?php
                    
$agrupadorHtml =
<<<EOF
    <table>
        <tr valign="middle">
            <td>
                <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <!--
                <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
                -->
                <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
            </td>
            <td>
                <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
            </td>
        </tr>
    </table>
    <script type="text/javascript" language="javascript">
        limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
        limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
        {POVOAR_ORIGEM}
        {POVOAR_DESTINO}
        sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
    </script>
EOF;
                    // inicia agrupador
                    $agrupador = new Agrupador( 'formulario', $agrupadorHtml );
                    $destino = array();
                    $origem = array(
                        'pais' => array(
                            'codigo'    => 'pais',
                            'descricao' => 'País'
                        ),
                        'regiao' => array(
                            'codigo'    => 'regiao',
                            'descricao' => 'Região'
                        ),
                        'unidade' => array(
                            'codigo'    => 'estado',
                            'descricao' => 'Unidade da Federação'
                        ),
                        'municipio' => array(
                            'codigo'    => 'municipio',
                            'descricao' => 'Município'
                        ),
                        'operadora' => array(
                            'codigo'    => 'operadora',
                            'descricao' => 'Operadora'
                        ),
                        'laboratorio' => array(
                            'codigo'    => 'laboratorio',
                            'descricao' => 'Laboratorio'
                        ),
                        'internet' => array(
                            'codigo'    => 'internet',
                            'descricao' => 'Internet'
                        ),
                        'prioridade' => array(
                            'codigo'    => 'prioridade',
                            'descricao' => 'Prioridade'
                        )
                        
                    );
                    // exibe agrupador
                    $agrupador->setOrigem( 'naoAgrupador', null, $origem );
                    $agrupador->setDestino( 'agrupador', null, $destino );
                    $agrupador->exibir();
                    
                ?>
            </td>
        </tr>
         
        <tr>
        <td>Filtros Territórios</td>
        </tr>
       <?/*
        <tr>
        <td class="SubTituloDireita" valign="top" width="20%">Relatórios Gerenciais:</td>
        <td></td>
        </tr>
        <tr>
        <td class="SubTituloDireita" valign="top" width="20%">Minhas Consultas:</td>
        <td></td>
        </tr>
*/
		?>
        <tr>
        <td class="SubTituloDireita" valign="top" width="20%">Região</td>
        <td>
        <?php
                $sqlComboRegiao = "
                    select
                        regcod as codigo,
                        regdescricao as descricao
                    from territorios.regiao
                    order by
                        regdescricao
                ";
                combo_popup( "regcod", $sqlComboRegiao, "Região", "192x400", 0, array(), "", "S", false, false, 5, 400 );
        ?>
        </td>
        </tr>
        <tr>
        <td class="SubTituloDireita" valign="top" width="20%">Unidade da Federação</td>
        <td>
        <?php
                $sqlComboEstado = "
                    select
                        estuf as codigo,
                        estdescricao as descricao
                    from territorios.estado
                    order by
                        estdescricao
                ";
                combo_popup( "estuf", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400 );
        ?>
        </td>
        </tr>
        
        <tr>
        <td class="SubTituloDireita" valign="top" width="20%">Mesoregião</td>
        <td>
        <?php
                $sqlComboMesoregiao = "
                   select
                       estuf,
                        mesdsc as descricao,
                        mescod as codigo
                    from territorios.mesoregiao
                    order by
                        estuf
                ";
                combo_popup( "mescod", $sqlComboMesoregiao, "Mesoregião", "400x400", 0, array(), "", "S", false, false, 5, 400 );
        ?>
        </td>
        </tr>
            <tr>
        <td class="SubTituloDireita" valign="top">Município</td>
        <td>
        <?php
                $sqlComboMunicipio = "
                    select mundescricao as nome,
                            estuf as siglaestado
                    from territorios.municipio
                    order by estuf
                ";
         
        ?>
        <select multiple="multiple" size="5" name="municipios[]" 
        id="municipios"  
        ondblclick="abrepopupMunicipio();"  
        class="CampoEstilo" style="width:400px;" >
        <option value="">Duplo clique para selecionar da lista</option>
        </select>
        </td>
    </tr>
    <?/*
        <tr>
        <td valign="top">Escolas</td>
        <td></td>
    </tr>
        <tr>
        <td class="SubTituloDireita" valign="top">Escolas</td>
        <td>
        <?php
                $sqlComboEscolas = "
                    select mundescricao as nome,
                            estuf as siglaestado
                    from territorios.municipio
                    order by estuf
                ";
                //combo_popup( "mundescricao", $sqlComboEscolas, "Município", "400x400", 0, array(), "", "S", false, false, 5, 400 );
        ?>
         <select multiple="multiple" size="5" name="escolas[]" 
        id="escolas"  
        ondblclick="abrepopupEscolas();"  
        class="CampoEstilo" style="width:400px;" >
        <option value="">Duplo clique para selecionar da lista</option>
        </select>
        </td>
    </tr>
*/
    ?>
        <tr>
        <td valign="top">Outros</td>
        <td>
        </td>
    </tr>
        <tr>
        <td class="SubTituloDireita" valign="top">Categoria de Prioridade</td>
        <td>
        <?php
                $sqlCategoriaPrioridade = "select tpadsc as codigo, tpaid || '. ' || tpadsc as descricao  
                from cte.tipoatendimento order by tpaid  
                ";
                combo_popup( "tpadsc", $sqlCategoriaPrioridade, "Prioridade", "400x400", 0, array(), "", "S", false, false, 5, 400 );
        ?>
       
        </td>
    </tr>
        <tr>
        <td class="SubTituloDireita" valign="top">Operadora</td>
        <td>
        <?php
        $sqlComboOp = "select oprdsc as codigo, oprdsc as descricao from cte.operadora
                    order by oprdsc";
        combo_popup( "oprdsc", $sqlComboOp, "Operadora", "192x400", 0, array(), "", "S", false, false, 5, 400 );    
        ?>
        </td>
    </tr>
        <tr>
        <td class="SubTituloDireita" valign="top">Laboratório Instalado?</td>
        <td>
        <input type="radio" name="labinstalado" id="labinstalado" value="true"/>
        <label for="fisico">Sim</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="labinstalado" id="labinstalado" value="false"/>
        <label for="fisico">Não</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="labinstalado" id="labinstalado" value=""/>
        <label for="fisico">Ambos</label>
        </td>
    </tr>
    <?/*
    <tr>
        <td class="SubTituloDireita" valign="top">Atendimento iniciado pela Operadora?</td>
        <td>
        <input type="radio" name="AtendimentoOP" id="AtendimentoOP" value="true"/>
        <label for="fisico">Sim</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="AtendimentoOP" id="AtendimentoOP" value="false"/>
        <label for="fisico">Não</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="AtendimentoOP" id="AtendimentoOP" value=""/>
        <label for="fisico">Ambos</label>
        </td>
    </tr>
	*/?>
        <tr>
        <td class="SubTituloDireita" valign="top">Data de Instalação da Banda Larga</td>
        <td>
        de <?= campo_data( 'atidatainicio', 'S', 'S', '', 'S' ); ?> (dd/mm/aaaa) a 
           <?= campo_data( 'atidatafim', 'S', 'S', '', 'S' ); ?> (dd/mm/aaaa)
        </td>
    </tr>
        <tr>
        <td class="SubTituloDireita" valign="top">Data de Previsão Instalação da Banda Larga</td>
        <td style="padding-left:18px;">
            <?= campo_data( 'dataprevinsta', 'S', 'S', '', 'S' ); ?> (dd/mm/aaaa)
        </td>
    </tr>
        <tr>
        <td class="SubTituloDireita" valign="top">Operadora possui área de Cobertura</td>
        <td>
        <input type="radio" name="Corbetura" id="Corbetura" value="true"/>
        <label>Sim</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="Corbetura" id="Corbetura" value="false"/>
        <label>Não</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="Corbetura" id="Corbetura" value=""/>
        <label>Ambos</label>
        </td>
    </tr>
     <tr>
        <td class="SubTituloDireita" valign="top">Banda larga instalada?</td>
        <td>
        <input type="radio" name="BLInstal" id="BLInstal" value="true"/>
        <label for="fisico">Sim</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="BLInstal" id="BLInstal" value="false"/>
        <label for="fisico">Não</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="BLInstal" id="BLInstal" value=""/>
        <label for="fisico">Ambos</label>
        </td>
    </tr>
     <tr>
        <td class="SubTituloDireita" valign="top">Inclui Universidade Aberta?</td>
        <td>
        <input type="radio" name="BLpolo" id="BLpolo" value="true"/>
        <label for="fisico">Sim</label>
        &nbsp;&nbsp;&nbsp;
        <input type="radio" name="BLpolo" id="BLpolo" value="false" checked="checked"/>
        <label for="fisico">Não</label>
        </td>
    </tr>    
        <tr>
        <td valign="top"></td>
        <td></td>
    </tr>
    <tr>
            <td class="SubTituloDireita" valign="top">
                &nbsp;
            </td>
            <td class="SubTituloDireita" style="text-align:left;">
                <input
                    type="button"
                    name="filtrar"
                    value="Visualizar"
                    onclick="exibirRelatorio('html');"
                />
                <input type="button" name="Exportar Planilha" value="Exportar Planilha" onclick="javascript:exibirRelatorio('xls');"/>
                <? /*
                <input
                    type="button"
                    name="gravar"
                    value="Gravar Consulta"
                    onclick=""
                />
                <input
                    type="button"
                    name="geraxls"
                    value="Gerar Arquivo XLS"
                    onclick=""
                />
*/
                ?>
            </td>
        </tr>
</table>
<input type="hidden" name="tipo" id="tipo" value="">
</form>

<script type="text/javascript">
	function abrepopupMunicipio(){
		window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/combo_municipios_bandalarga.php','Municipios','width=400,height=400,scrollbars=1');
	}
	
	function abrepopupEscolas(){
		window.open('http://<?=$_SERVER['SERVER_NAME']?>/cte/combo_escolas_bandalarga.php','Escolas','width=400,height=400,scrollbars=1');
	}
	

    function exibirRelatorio(valortipo)
    {
        var formulario = document.formulario;
        formulario.tipo.value = valortipo;
        // verifica se tem algum agrupador selecionado
        agrupador = document.getElementById( 'agrupador' );
        if ( !agrupador.options.length )
        {
            alert( 'Escolha ao menos um item para agrupar o resultado.' );
            return;
        }
        
        selectAllOptions( agrupador );
        selectAllOptions( document.getElementById( 'regcod' ) );
        selectAllOptions( document.getElementById( 'estuf' ) );
        selectAllOptions( document.getElementById( 'mescod' ) );
        selectAllOptions( document.getElementById( 'municipios' ) );
        selectAllOptions( document.getElementById( 'tpadsc' ) );
        selectAllOptions( document.getElementById( 'oprdsc' ) );
     
        
        // submete formulario
        //alert( 'blz!' ); return;
        formulario.target = 'bandalargaresultado';
        var janela = window.open( '', 'bandalargaresultado', 'width=1024,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
        formulario.post.value = 'ok';
        formulario.submit();
        janela.focus();
    }

</script>

