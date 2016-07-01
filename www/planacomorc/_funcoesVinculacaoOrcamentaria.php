<?php

/**
 * Adiciona aspas aos valores vindos por GET
 * @return String
 */
function getRequestValues() {
    $arrValues = explode(',', $_GET['values']);
    $tmp_Values = array();
    foreach ($arrValues as $value) {
        $tmp_Values[] = "'{$value}'";
    }
    return implode(',', $tmp_Values);
}

/**
 * Adiciona aspas aos valores do array passado
 * @param type $values
 * @return type
 */
function addSlash($values) {
    $arrValues = explode(',', $values);
    $tmp_Values = array();
    foreach ($arrValues as $value) {
        $tmp_Values[] = "'{$value}'";
    }
    return implode(',', $tmp_Values);
}

/**
 * Monta um input para ser usado pelo "pesquisator" do jQuery
 * @param string $name
 * @param string $label
 * @return HTML
 */
function inputSearch($name, $label='Pesquisar:') {
    $attr = 'border="1" style="border-collapse:collapse;" bgcolor="#f5f5f5" cellSpacing="4" cellPadding="4" align="center"';
    $echo = <<<HTML
        <table class="tabela table table-striped table-bordered table-hover {$attr}">
            <tr>
                <td width="10%" style="vertical-align:middle;font-weight:bold;">{$label}</td>
                <td colspan="2">
                    <input type="text" name="{$name}" size="40" class="{$name} normal form-control" />
                </td>
            </tr>
        </table>
HTML;
    return $echo;
}

function scriptAcoes(){
    echo <<<JAVASCRIPT
        <script>
            $(".superCheck").click(function() {
    
                var section = $("." + $(this).attr("data-target"))
                        , tr;
                
                if ($(this).is(":checked") === true) {
                    section.each(function(i, element) {
                        tr = $($(element).parent().parent());
                        if (!tr.hasClass("remover")) {
                            $(element).prop("checked", true);
                        }
                    });
                } else {
                    section.each(function(i, element) {
                        $(element).prop("checked", false);
                    });
                }
            });

            $('input.unicodSeach').on('keyup',function() {
    
                $('table.unicodSeach tbody tr td').removeClass('marcado');
                $('table.unicodSeach tbody tr').removeClass('remover');
                var stringPesquisa = $(this).val();
                if (stringPesquisa) {
                    $('table.unicodSeach tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                    $('table.unicodSeach tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
                }
            });
            
            $("input.uoFilter").keyup(function() {

                $('table.uoFilter tbody tr td').removeClass('marcado');
                $('table.uoFilter tbody tr').removeClass('remover');
                var stringPesquisa = $(this).val();
                if (stringPesquisa) {
                    $('table.uoFilter tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                    $('table.uoFilter tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
                }
            });
    
            $("input.sbaFilter").keyup(function() {
    
                $('table.sbaFilter tbody tr td').removeClass('marcado');
                $('table.sbaFilter tbody tr').removeClass('remover');
                var stringPesquisa = $(this).val();
                if (stringPesquisa) {
                    $('table.sbaFilter tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                    $('table.sbaFilter tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
                }
            });
    
    //Teste
            $.expr[':'].contains = function(a, i, m) {
                return $(a).text().toUpperCase().indexOf(m[3].toUpperCase()) >= 0;
            };
            
            
            $("input.acaoUoCheck").keyup(function() {

                $('table.acaoUoCheck tbody tr td').removeClass('marcado');
                $('table.acaoUoCheck tbody tr').removeClass('remover');
                var stringPesquisa = $(this).val();
                if (stringPesquisa) {
                    $('table.acaoUoCheck tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                    $('table.acaoUoCheck tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
                }
            });
            $("input.subacaoCheck").keyup(function() {

                $('table.subacaoCheck tbody tr td').removeClass('marcado');
                $('table.subacaoCheck tbody tr').removeClass('remover');
                var stringPesquisa = $(this).val();
                if (stringPesquisa) {
                    $('table.subacaoCheck tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                    $('table.subacaoCheck tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
                }
            });
            $("input.plicodCheck").keyup(function() {

                $('table.plicodCheck tbody tr td').removeClass('marcado');
                $('table.plicodCheck tbody tr').removeClass('remover');
                var stringPesquisa = $(this).val();
                if (stringPesquisa) {
                    $('table.plicodCheck tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                    $('table.plicodCheck tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
                }
            });
            $("input.ptresCheck").keyup(function() {

                $('table.ptresCheck tbody tr td').removeClass('marcado');
                $('table.ptresCheck tbody tr').removeClass('remover');
                var stringPesquisa = $(this).val();
                if (stringPesquisa) {
                    $('table.ptresCheck tbody tr td:contains(' + stringPesquisa + ')').addClass('marcado');
                    $('table.ptresCheck tbody tr:not(:contains(' + stringPesquisa + '))').addClass('remover');
                }
            });
            
        </script>
JAVASCRIPT;
}

/**
 * Monta clausa de multiplos inserts
 * @param type $uos
 * @param type $acoes
 * @param type $vaeid
 * @return string
 */
function insert_values_acoes($acoes, $vaeid) {
    $acoes = explode(',', $acoes);
    $inserts = '';
    #$data = array();
    foreach ($acoes as $acao) {
        $valor = explode('-', $acao);
        $inserts .= "('{$valor[1]}', '{$vaeid}' ,'{$valor[0]}'),";
        #$data[] = array('acacod' => $valor[1], 'vaeid' => $vaeid, 'unicod' => $valor[0]);
    	//print_r($inserts);
    }
    return substr($inserts,0,-1);
}

/**
 * Monta clausa de multiplos inserts
 * @param type $uos
 * @param type $acoes
 * @param type $vaeid
 * @return type
 */
function insert_values_subacoes($uos, $acoes, $vaeid) {
    $uos = explode(',', $uos);
    $acoes = explode(',', $acoes);    
    $inserts = '';
    foreach ($uos as $k => $uo) {
        foreach ($acoes as $acao) {
            $inserts .= "($vaeid, '{$acao}', '{$uo}'),";
            #$data[] = array('vaeid' => $vaeid, 'sbacod' => $acao, 'unicod' => $uo);
        }
    }    
    return substr($inserts,0,-1);
}

/**
 * Monta clausa de multiplos inserts
 * @param type $uos
 * @param type $acoes
 * @param type $vaeid
 * @return type
 */
function insert_values_pis($uos, $acoes, $vaeid) {
    $uos = explode(',', $uos);
    $acoes = explode(',', $acoes);
    $inserts = '';
    foreach ($uos as $k => $uo) {
        if ($k===0) $inserts .= '(';
        foreach ($acoes as $acao) {
            $inserts .= "$vaeid, '{$acao}', '{$uo}'),(";
        }
    }
    return substr($inserts,0,-2);
}

/**
 * Monta clausa de multiplos inserts
 * @param type $uos
 * @param type $acoes
 * @param type $vaeid
 * @return type
 */
function insert_values_pos($acoes, $vaeid) {
    $acoes = explode(',', $acoes);
    $inserts = '';
    foreach ($acoes as $acao) {
        $parts = explode('-', $acao);
        $inserts .= "('{$parts[0]}', '{$parts[1]}', '', $vaeid, '{$parts[2]}'),";
    }

    return substr($inserts,0,-1);
}

/**
 * Monta array com clausula where
 * @param type $uos
 * @param type $acoes
 * @param type $vaeid
 * @return array
 */
function select_values_acoes($acoes, $vaeid) {
    $acoes = explode(',', $acoes);
    $where = array();
    foreach ($acoes as $acao) {
        $parts = explode('-',$acao);
        $where[] = "acacod='{$parts[1]}' AND vaeid='{$vaeid}' AND unicod='{$parts[0]}'";
    }
    return $where;
}

function select_values_subacoes($uos, $acao, $vaeid) {
    $uos = explode(',', $uos);
    $where = array();
    foreach ($uos as $uo) {
        $where[] = "sbacod='{$acao}' AND vaeid='{$vaeid}' AND unicod='{$uo}'";
    }
    return $where;
}

function select_values_pi($acao, $vaeid) {
    return "plicod='{$acao}' AND vaeid='{$vaeid}'";
}

function select_values_po($acoes, $vaeid) {
    $acoes = explode(',', $acoes);
    $where = array();

    foreach ($acoes as $acao) {
        $parts = explode('-', $acao);
        $where[] = "ptres='{$parts[0]}' AND vaeid='{$vaeid}' AND unicod='{$parts[2]}'";
    }
    return $where;
}

function superCheck($name) {
    return sprintf('<input type="checkbox" class="superCheck" data-target="'.$name.'"/>');
}

/**
 * Insert OR Update
 * @global type $db
 * @param array $post
 * @return type
 */
function salvar(array $post) {
    global $db;
    extract($post);
    
    if (empty($vacid) && !empty($acaid) && !empty($exercicio) && !empty($vaetituloorcamentario) && !empty($vaedescricao)) {

        $instruction = "INSERT INTO planacomorc.vinculacaoacaoestrategica
        (acaid,  vaetituloorcamentario, vaedescricao)
        VALUES($acaid, '$vaetituloorcamentario', '$vaedescricao') returning vacid";
        $vacid = $db->pegaUm($instruction);
        
        $instruction = "INSERT INTO planacomorc.vinculacaoacaoestrategicaexercicio
        (vacid,vaedescricao,vaetituloorcamentario, exercicio, acaid)
        VALUES($vacid, '$vaedescricao', '$vaetituloorcamentario', '$exercicio' , '$acaid') returning vaeid";
        $vaeid = $db->pegaUm($instruction);        
    } else {
        $instruction = "UPDATE planacomorc.vinculacaoacaoestrategica SET %s WHERE vacid=%d";
        $cleanup = array('acaid', 'vaetituloorcamentario', 'vaedescricao');        
        $set = '';        
        foreach ($post as $k => $v) {
            if (in_array($k, $cleanup) && !empty($post[$k])) $set.= "$k = '{$v}',";
        }        
        $set = substr($set, 0, -1);
        if (!empty($set) && is_numeric($vacid)) {
            $stmt = sprintf($instruction, $set, $vacid);
            //ver($stmt,d);
            $db->executar($stmt);
        }
        
        $instruction = "UPDATE planacomorc.vinculacaoacaoestrategicaexercicio SET %s WHERE vacid=%d and exercicio='{$_REQUEST['exercicio']}'";        
        $cleanup = array('acaid', 'vaedescricao','acaid', 'vaetituloorcamentario');        
        $set = '';        
        foreach ($post as $k => $v) {
            if (in_array($k, $cleanup) && !empty($post[$k])) $set.= "$k = '{$v}',";
        }        
        $set = substr($set, 0, -1);
        if (!empty($set) && is_numeric($vacid)) {
            $stmt = sprintf($instruction, $set, $vacid);

            $db->executar($stmt);
        }
    }
    
    $db->commit();
    alertlocation(array(
        'alert' => 'Registro foi salvo com sucesso!'
      , 'location' => 'planacomorc.php?modulo=sistema/tabelasapoio/vinculacaoOrcamentaria&acao=A'
    ));
}

function _monta_lista($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$valormonetario="S",$nomeformulario="",$celWidth="",$celAlign="",$tempocache=null,$param=Array()) {
    // este mtodo monta uma listagem na tela baseado na sql passada (tem que estar fora de tags FORM'S)
    //$sql = Texto - sql que vai gerar a lista
    //$cabecaho = Vetor - contendo o nome que vai ser exibido, deve ter a mesma quantidade dos campos da sql
    //Parmetros de paginao
    //$perpage = Numrico - Registros por pgina
    //$pages = Numrico - Numrico - Mx de Paginas que sero mostradas na barrinha de paginao
    //$soma = Boleano - Mostra somatrio de campos numricos no fim da lista
    //$ordem = alinhamento dos títulos (left, rigth, center)
    //$valormonetario = Define se o valor a ser tratado é monetário (com casas decimais) ou numérico (sem casas decimais)
    //$nomeformulario = Caso seja necessário um formulário para acessar objetos dentro da tabela, informar o nome do formulário, que o componente irá criar um <form name="nomedoformularioinformado">...</form>
    //Registro Atual (instanciado na chamada)
    //$sql = str_replace(";", "", $sql);

    /**************************************************************************************************
     -> NOVOS PARAMETROS (programador: Felipe Carvalho / data: 24/07/2009)	
     $celWidth => array com o width(tamanho) de cada célula. Pode-se usar qualquer unidade (px, %, pt, etc)		
     $celAlign => array com o alinhamento de cada célula.

     NOS DOIS PARAMETROS O TAMANHO DO ARRAY DEVE SER O MESMO DO	
     NUMERO DE ELEMENTOS QUE IRAO COMPOR A LISTA.					

     PARAMETROS NAO OBRIGATORIOS!
     SE NAO FOREM INFORMADOS USA-SE O PADRAO DO COMPONENTE.								

     EXEMPLO:														
            $cabecalho 		= array( "Comando", "Ação"); ou Array('comando',Array('label'=>'parte de cima','colunas'=>Array('subcoluna1','subcoluna2')))				
            $tamanho		= array( '10%', '90%' );															
            $alinhamento	= array( 'center', 'left' );													
            $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
    **************************************************************************************************/
    /**************************************************************************************************
     -> NOVOS PARAMETROS (programador: Eduardo Dunice / data: 08/09/2012)	
     $param => array que comporta vários parametros extras.

            Opções suportadas:
            - ordena 	   -> ativa e desativa algoritmo de ordenação do monta lista (contorna bug com SQL q possui subselect com clausula ORDER BY).
            - totalLinhas  -> ativa e desativa rodapé com total de linhas listadas.
            - managerOrder -> deve conter os parâmetros de ordenamento usando como índice o número correspondente a coluna (iniciando de 1) 
            - classTable -> deve conter os parâmetros do tipo (string) para adiciona classes css adicionais para a tabela gerada

     PARAMETROS NAO OBRIGATORIOS!

     EXEMPLO:														
            $param['ordena'] 	   = (true ou false);																
            $param['totalLinhas']  = (true ou false);												
            $param['managerOrder'] = array(12 => 'o.obrnome',
                                                                                    15 => 'oc.ocrdtinicioexecucao',
                                                                                    16 => 'oc.ocrdtterminoexecucao',
                                                                                    18 => array('campo' => "DATE_PART('days', NOW() - o.obrdtultvistoria)", 'alias' => "obrdtultvistoria"),
                                                                                    19 => 'obrdtultvistoria'
            $param['classTable'] = 'lista listagem listaClica'
                                                                               ); 												
            $db->monta_lista($sql, $cabecalho, 50, 20, '', '100%', '', '', '', '', '', $param);
    **************************************************************************************************/

    global $db;

    $ordena = isset($param['ordena']) ? $param['ordena'] : true;
    $param['totalLinhas'] = is_bool($param['totalLinhas']) ? $param['totalLinhas'] : true;

    if ($_REQUEST['numero']=='') $numero = 1; else $numero = intval($_REQUEST['numero']);	
    //Controla o Order by
    if (!is_array($sql) && $_REQUEST['ordemlista']<>'' && $ordena)
    {
            if ($_REQUEST['ordemlistadir'] <> 'DESC') {
                    $ordemlistadir  = 'ASC';
                    $ordemlistadir2 = 'DESC';
            } else {
                    $ordemlistadir  = 'DESC'; 
                    $ordemlistadir2 = 'ASC';
            }

            $subsql 	= substr($sql,0,strpos(trim(strtoupper($sql)),'ORDER '));

            if ( !empty($param['managerOrder'][$_REQUEST['ordemlista']]) ){
                    if ( is_array( $param['managerOrder'][$_REQUEST['ordemlista']] ) ){
                            $campoOrder = $param['managerOrder'][$_REQUEST['ordemlista']]['campo']; 
                            $aliasOrder = $param['managerOrder'][$_REQUEST['ordemlista']]['alias']; 
                    } else {
                            $campoOrder = $param['managerOrder'][$_REQUEST['ordemlista']]; 
                            $aliasOrder = $param['managerOrder'][$_REQUEST['ordemlista']]; 
                    }
                    $ordemLista = $aliasOrder . '_oculto';

                    $strReplace = 'SELECT';
                    if ( strpos( trim(strtoupper($subsql)), 'DISTINCT' ) ){
                            $posIni 	    = strpos( trim(strtoupper($subsql)), 'SELECT' ) + 6;
                            $posFim 	    = strpos( trim(strtoupper($subsql)), 'DISTINCT' ) - 6;
                            $espacoDistinct = trim(substr($subsql, $posIni, $posFim) );

                            if ( empty($espacoDistinct) ){
                                    $strReplace = 'DISTINCT';	
                            }
                    }
                    $subsql = preg_replace('/' . $strReplace . '/i', 
                                                               $strReplace . " " . $campoOrder . ' AS ' . $aliasOrder . '_oculto, ',
                                                               $subsql,
                                                               1);
            }else{
                    $ordemLista = $_REQUEST['ordemlista'];
            }

            $sql = (!$subsql ? $sql : $subsql).' order by '.$ordemLista.' '.$ordemlistadir;
    }

    if (is_array($sql)){
        $RS = $sql;
        $totalRegistro = count($RS);
        }else{
                $sql = trim($sql);
                $char = substr($sql, -1); 
                if ($char == ";") $sql = substr($sql, 0, -1);

                $sqlCount = "select count(1) from (" . $sql . ") rs";

                $totalRegistro = $db->pegaUm($sqlCount,0,$tempocache);

                $sql = $sql . " LIMIT {$perpage} offset ".($numero-1);	

                $RS = $db->carregar($sql,null,$tempocache);
    }
    $nlinhas = count($RS);

    if (! $RS) $nl = 0; else $nl=$nlinhas;
//	if (($numero+$perpage)>$nlinhas) $reg_fim = $nlinhas; else $reg_fim = $numero+$perpage-1;
    $reg_fim = $nlinhas;
    if ($nl>0)
    {
            $total_reg = $totalRegistro;

            //monta o formulario da lista mantendo os parametros atuais da pgina
            print '<form name="formlista" method="post"><input type="Hidden" name="numero" value="" /><input type="Hidden" name="ordemlista" value="'.$_REQUEST['ordemlista'].'"/><input type="Hidden" name="ordemlistadir" value="'.$ordemlistadir.'"/>';

            foreach($_POST as $k=>$v){
                    if ($k<>'ordemlista' and $k<>'ordemlistadir' and $k<>'numero'){
                            if( is_array($v)){
                                    foreach($v as $k2 => $v2){
                                            if( is_array($v2)){
                                                    foreach($v2 as $k3 => $v3){
                                                            print '<input type="Hidden" name="'.$k.'['.$k2.']['.$k3.']" value="'.$v3.'"/>';
                                                    }
                                            }else{
                                                    print '<input type="Hidden" name="'.$k.'['.$k2.']" value="'.$v2.'"/>';
                                            }
                                    }
                            }else{
                                    print '<input type="Hidden" name="'.$k.'" value="'.$v.'"/>';
                            }
                    }
            }

            print '</form>';

            if($nomeformulario != "")
                    print '<form name="'.$nomeformulario.'" id="'.$nomeformulario.'" action="" enctype="multipart/form-data" method="post">';

            $classTable = (array_key_exists('classTable', $param)) ? $param['classTable'] : '';
            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem '.$classTable.'">';
            //Monta Cabealho
            if ( $cabecalho === null ) {

            }else if(is_array($cabecalho))
            {
                    foreach($cabecalho as $cab){
                            if(is_array($cab)){
                                    $cab_rowspan = " rowspan='2' ";
                            }
                    }

                    print '<thead><tr>';
                    for ($i=0;$i<count($cabecalho);$i++)
                    {
                            if(!is_array($cabecalho[$i])){
                                    if ($_REQUEST['ordemlista'] == ($i+1)) {
                                            $ordemlistadirnova = $ordemlistadir2;
                                            $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                                    } else {
                                            $ordemlistadirnova = 'ASC';
                                            $imgordem = '';
                                    }
                                    $arrColunasComuns[] = 1;
                                    $onclick = $ordena ? 'onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');"' : '';
                                    print '<td '.$cab_rowspan.' align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" '.$onclick.' title="Ordenar por '.strip_tags($cabecalho[$i]).'">'.$imgordem.'<strong>'.$cabecalho[$i].'</strong></label>';
                            }else{
                                    print '<td colspan="'.count($cabecalho[$i]['colunas']).'"  align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" ><center><strong>'.$cabecalho[$i]['label'].'</strong></center></label>';
                                    foreach($cabecalho[$i]['colunas'] as $col_b){
                                            $arrColunasDebaixo[] = $col_b;
                                    }
                            }
                    }
                    if($arrColunasDebaixo){
                            print "<tr>";
                            $i = count($arrColunasComuns);
                            foreach($arrColunasDebaixo as $colunaDeBaixo){
                                    if ($_REQUEST['ordemlista'] == $i+1) {
                                            $ordemlistadirnova = $ordemlistadir2;
                                            $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                                    } else {
                                            $ordemlistadirnova = 'ASC';
                                            $imgordem = '';
                                    }
                                    print '<td align="' . $alinha . '" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.strip_tags($colunaDeBaixo).'">'.$imgordem.'<strong>'.$colunaDeBaixo.'</strong></label>';
                                    $i++;
                            }
                    }
                    print '</tr> </thead>';
            }
            else
            {
                    print '<thead><tr>'; $i=0;
                    foreach($RS[0] as $k=>$v)
                    {
                            if ($_REQUEST['ordemlista'] == ($i+1)) {
                                    $ordemlistadirnova = $ordemlistadir2;
                                    $imgordem = '<img src="../imagens/seta_ordem'.$ordemlistadir.'.gif" width="11" height="13" align="middle"> ';
                            } else {
                                    $ordemlistadirnova = 'ASC';
                                    $imgordem = '';}
                                    print '<td valign="top" class="title" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';" onclick="ordena(\''.($i+1).'\',\''.$ordemlistadirnova.'\');" title="Ordenar por '.strip_tags($k).'">'.$imgordem.'<strong>'.$k.'</strong></label>';
                                    $i=$i+1;}
                                    print '</tr> </thead>';
            }
            //Monta Listagem
            $totais = array();
            $tipovl = array();
//		for ($i=($numero-1);$i<$reg_fim;$i++)

            for ($i=0; $i < $reg_fim; $i++)
            {
                    $c = 0;
                    if (fmod($i,2) == 0) $marcado = '' ; else $marcado='#F7F7F7';
                    print '<tr bgcolor="'.$marcado.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$marcado.'\';">';

                    // contador -> numero de celulas
                    $numCel = 0;
                    foreach($RS[$i] as $k=>$v) {
                            if ( strpos($k, '_oculto') ){
                                    continue;
                            }

                            // Setando o alinhamento da célula usando o array $celAlign.
                            // Se não for passado o parâmetro, usa o padrão do componente.
                            if(is_array($celAlign)) {
                                    $alignNumeric = $alignNotNumeric = $celAlign[$numCel];
                            } else {
                                    $alignNumeric 		= 'right';
                                    $alignNotNumeric	= 'left';
                            }
                            // Setando o tamanho da célula usando o array $celWidth.
                            // Se não for passado o parâmetro, usa o padrão do componente.
                            $width = (is_array($celWidth)) ? 'width="'.$celWidth[$numCel].'"' : '';

                            if (is_numeric($v))
                            {
                                    //cria o array totalizador
                                    if (!$totais['0'.$c]) {$coluna = array('0'.$c => $v); $totais = array_merge($totais, $coluna);} else $totais['0'.$c] = $totais['0'.$c] + $v;
                                    //Mostra o resultado
                                    if (strpos($v,'.')) {$v = number_format($v, 2, ',', '.'); if (!$tipovl['0'.$c]) {$coluna = array('0'.$c => 'vl'); $tipovl = array_merge($totais, $coluna);} else $tipovl['0'.$c] = 'vl';}
                                    if ($v<0) print '<td align="'.$alignNumeric.'" '.$width.' style="color:#cc0000;" title="'.strip_tags($cabecalho[$c]).'">('.$v.')'; else print '<td align="'.$alignNumeric.'" '.$width.' style="color:#0066cc;" title="'.strip_tags($cabecalho[$c]).'">'.$v;
                                    print ('<br>'.$totais[$c]);
                            }
                            else {
                                    print '<td align="'.$alignNotNumeric.'" '.$width.' title="'.strip_tags($cabecalho[$c]).'">'.$v;
                            }
                            print '</td>';
                            $c = $c + 1;
                            $numCel++;
                    }
                    print '</tr>';
            }

            if ($soma=='S'){
                    //totaliza (imprime totais dos campos numericos)
                    print '<thead><tr>';
                    for ($i=0;$i<$c;$i++)
                    {
                            print '<td align="right" title="'.strip_tags($cabecalho[$i]).'">';

                            if ($i==0) print 'Totais:   ';
                            if (is_numeric($totais['0'.$i])) {

                            if($valormonetario == 'S'){
                                                    print number_format($totais['0'.$i], 2, ',', '.'); 
                                            }else{
                                                    print $totais['0'.$i]; 
                                            }
                            }
                                    else print $totais['0'.$i];
                            print '</td>';
                    }
                    print '</tr>';
                    //fim totais
            }

            print '</table>';

            if($nomeformulario != "")
                    print '</form>';
            if( $param['totalLinhas'] ){
                    print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem"><tr bgcolor="#ffffff"><td><b>Total de Registros: ' . $totalRegistro . '</b></td><td>';
    print '</td></tr></table>';
            }

            include APPRAIZ."includes/paginacao.inc";

            print '<script language="JavaScript">function ordena(ordem, direcao) {document.formlista.ordemlista.value=ordem;document.formlista.ordemlistadir.value=direcao;document.formlista.submit();} function pagina(numero) {document.formlista.numero.value=numero;document.formlista.submit();}</script>';
    }
    else
    {
            print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
            print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
            print '</table>';
    }
}


function retornaSubacoesPeloAnoPtres($exercicio, $selecionados = FALSE, $unidades = null)
{
    global $db;
    if($selecionados){
        $andSelecionados = "
            AND sbacod NOT IN (
                SELECT sbacod
                FROM planacomorc.vinculacaoestrategicasubacoes 
                WHERE vaeid = '{$_REQUEST['id']}' AND unicod IN ({$unidades}) 
            )
        ";
    }
    
    if($unidades){
        $andUnidades = "
            AND pt.unicod IN ($unidades)
        ";
    }
    
    $strSQL_subacoes = <<<DML
        SELECT distinct 
            '<input type=\"checkbox\" name=\"subacao[]\" class=\"subacaoCheck\" value=\"'||sbacod||'\"/>' AS subacao, 
            sbacod, 
            '<span class=\"text-primary\">'|| COALESCE(sbasigla, 'N/A')||'</span>' || ' - ' || sbatitulo AS descricao
        FROM monitora.pi_subacao sb
        JOIN monitora.pi_subacaodotacao sd ON sd.sbaid = sb.sbaid
        JOIN monitora.ptres pt ON pt.ptrid = sd.ptrid AND pt.ptrano = sb.sbaano
        WHERE sb.sbaano = '{$exercicio}'
            {$andUnidades}
            {$andSelecionados}
            
DML;
    #ver($strSQL_subacoes);
    if ($rs = $db->carregar($strSQL_subacoes)) {        	
        return $rs;
    } else {
        return false;
    }       
}

function cotacaoAcoes($vaeid ){
    if(!$vaeid){ return false; }
    
    $query = <<<DML
        SELECT
            COALESCE(sum(vlrempenhado),0.00) as empenhado,     
            COALESCE(sum(vlrpago),0.00) as pago,
            COALESCE(sum(vlrrapnaoprocessadopago),0.00) as rapnp_ppago,
            COALESCE(sum(vlrrapprocessadopago),0.00) as rap_ppago            
        FROM spo.siopexecucao sp
        INNER JOIN (SELECT DISTINCT acacod,unicod FROM planacomorc.vinculacaoestrategicaacoes WHERE vaeid = $vaeid) vaa ON (vaa.acacod = sp.acacod AND vaa.unicod = sp.unicod)
        WHERE sp.exercicio = '{$_REQUEST['clicouexercicio']}'
DML;
    global $db;
    $cotacao = $db->pegaLinha($query);
    if(count($cotacao) > 0){
        $empenhado = number_format($cotacao['empenhado'], 2, ',', '.');
        $pago = number_format($cotacao['pago'], 2, ',', '.');
        $rap_np = number_format($cotacao['rapnp_ppago'], 2, ',', '.');
        $rap_p = number_format($cotacao['rap_ppago'], 2, ',', '.');
        $html = <<<HTML
            <table style="margin-bottom:0;" class="tabela table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
                <tr>
                    <td><span class="red">Despesas Empenhadas:</span> <span class="bold">$empenhado</span></td>
                    <td><span class="red">Valores Pagos:</span> <span class="bold">$pago</span></td>
                    <td><span class="red">RAP não-Processados Pagos:</span> <span class="bold">$rap_np</span></td>
                    <td><span class="red">RAP Processados Pagos:</span> <span class="bold">$rap_p</span></td>
                </tr>
            </table>
HTML;
    } 
    return $html;
}

function cotacaoSubacoes($vaeid ){
    if(!$vaeid){ return false; }
    
    $query = <<<DML
        SELECT 
            COALESCE(sum(vlrempenhado),0.00) as empenhado,     
            COALESCE(sum(vlrpago),0.00) as pago,
            COALESCE(sum(vlrrapnaoprocessadopago),0.00) as rapnp_ppago,
            COALESCE(sum(vlrrapprocessadopago),0.00) as rap_ppago
        FROM spo.siopexecucao sp
	INNER JOIN (SELECT DISTINCT sbacod,unicod FROM planacomorc.vinculacaoestrategicasubacoes WHERE vaeid = '{$vaeid}') vaa ON (vaa.sbacod =  substr(sp.plicod, 2,4) AND vaa.unicod = sp.unicod)
        WHERE sp.exercicio = '{$_REQUEST['clicouexercicio']}'
DML;
    global $db;
    $cotacao = $db->pegaLinha($query);
    if(count($cotacao) > 0){
        $empenhado = number_format($cotacao['empenhado'], 2, ',', '.');
        $pago = number_format($cotacao['pago'], 2, ',', '.');
        $rap_np = number_format($cotacao['rapnp_ppago'], 2, ',', '.');
        $rap_p = number_format($cotacao['rap_ppago'], 2, ',', '.');
        $html = <<<HTML
            <table style="margin-bottom:0;" class="tabela table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
                <tr>
                    <td><span class="red">Despesas Empenhadas:</span> <span class="bold">$empenhado</span></td>
                    <td><span class="red">Valores Pagos:</span> <span class="bold">$pago</span></td>
                    <td><span class="red">RAP não-Processados Pagos:</span> <span class="bold">$rap_np</span></td>
                    <td><span class="red">RAP Processados Pagos:</span> <span class="bold">$rap_p</span></td>
                </tr>
            </table>
HTML;
    }else{
        $html = <<<HTML
            <span class="label label-info">Nenhuma subação adicionada</span>
HTML;
    }
    return $html;
}

function cotacaoPI($vaeid ){
    if(!$vaeid){ return false; }
    
    $query = <<<DML
        SELECT 
            COALESCE(sum(vlrempenhado),0.00) as empenhado,     
            COALESCE(sum(vlrpago),0.00) as pago,
            COALESCE(sum(vlrrapnaoprocessadopago),0.00) as rapnp_ppago,
            COALESCE(sum(vlrrapprocessadopago),0.00) as rap_ppago
        FROM spo.siopexecucao sp
        INNER JOIN (SELECT DISTINCT plicod,unicod FROM planacomorc.vinculcaoestrategicapis WHERE vaeid = '{$vaeid}') vaa ON (vaa.plicod =  sp.plicod AND vaa.unicod = sp.unicod)
        WHERE sp.exercicio = '{$_REQUEST['clicouexercicio']}'
DML;
    global $db;
    $cotacao = $db->pegaLinha($query);
    if(count($cotacao) > 0){
        $empenhado = number_format($cotacao['empenhado'], 2, ',', '.');
        $pago = number_format($cotacao['pago'], 2, ',', '.');
        $rap_np = number_format($cotacao['rapnp_ppago'], 2, ',', '.');
        $rap_p = number_format($cotacao['rap_ppago'], 2, ',', '.');
        $html = <<<HTML
            <table style="margin-bottom:0;" class="tabela table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
                <tr>
                    <td><span class="red">Despesas Empenhadas:</span> <span class="bold">$empenhado</span></td>
                    <td><span class="red">Valores Pagos:</span> <span class="bold">$pago</span></td>
                    <td><span class="red">RAP não-Processados Pagos:</span> <span class="bold">$rap_np</span></td>
                    <td><span class="red">RAP Processados Pagos:</span> <span class="bold">$rap_p</span></td>
                </tr>
            </table>
HTML;
    }else{
        $html = <<<HTML
            <span class="label label-info">Nenhuma subação adicionada</span>
HTML;
    }
    return $html;
}

function cotacaoPTRES($vaeid ){
    if(!$vaeid){ return false; }
    
    $query = <<<DML
        SELECT 
            COALESCE(sum(vlrempenhado),0.00) as empenhado,     
            COALESCE(sum(vlrpago),0.00) as pago,
            COALESCE(sum(vlrrapnaoprocessadopago),0.00) as rapnp_ppago,
            COALESCE(sum(vlrrapprocessadopago),0.00) as rap_ppago
        FROM spo.siopexecucao sp
        INNER JOIN (SELECT ptres,unicod FROM planacomorc.vinculacaoestrategicapos WHERE vaeid = '{$vaeid}') vaa ON (vaa.ptres =  sp.ptres AND vaa.unicod = sp.unicod)
        WHERE sp.exercicio = '{$_REQUEST['clicouexercicio']}'
DML;
    global $db;
    $cotacao = $db->pegaLinha($query);
    if(count($cotacao) > 0){
        $empenhado = number_format($cotacao['empenhado'], 2, ',', '.');
        $pago = number_format($cotacao['pago'], 2, ',', '.');
        $rap_np = number_format($cotacao['rapnp_ppago'], 2, ',', '.');
        $rap_p = number_format($cotacao['rap_ppago'], 2, ',', '.');
        $html = <<<HTML
            <table style="margin-bottom:0;" class="tabela table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
                <tr>
                    <td><span class="red">Despesas Empenhadas:</span> <span class="bold">$empenhado</span></td>
                    <td><span class="red">Valores Pagos:</span> <span class="bold">$pago</span></td>
                    <td><span class="red">RAP não-Processados Pagos:</span> <span class="bold">$rap_np</span></td>
                    <td><span class="red">RAP Processados Pagos:</span> <span class="bold">$rap_p</span></td>
                </tr>
            </table>
HTML;
    }else{
        $html = <<<HTML
            <span class="label label-info">Nenhuma subação adicionada</span>
HTML;
    }
    return $html;
}

function cotacaoTotal($vaeid, $exercicio){
    global $db;
    $sql = <<<DML
        SELECT 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrempenhado) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.acacod::text IN 
                                    (SELECT DISTINCT (vinculacaoestrategicaacoes.unicod::text || '.'::text) || vinculacaoestrategicaacoes.acacod::text
                                    FROM planacomorc.vinculacaoestrategicaacoes
                                    WHERE vinculacaoestrategicaacoes.vaeid = ve.vaeid)
                            )
                    ), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrempenhado) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || substr(siopexecucao.plicod::text, 2, 4) IN 
                                    (SELECT DISTINCT (vinculacaoestrategicasubacoes.unicod::text || '.'::text) || vinculacaoestrategicasubacoes.sbacod::text
                                    FROM planacomorc.vinculacaoestrategicasubacoes
                                    WHERE vinculacaoestrategicasubacoes.vaeid = ve.vaeid)
                            )
                    ), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrempenhado) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.plicod::text IN 
                                    (SELECT DISTINCT (vinculcaoestrategicapis.unicod::text || '.'::text) || vinculcaoestrategicapis.plicod::text
                                    FROM planacomorc.vinculcaoestrategicapis
                                    WHERE vinculcaoestrategicapis.vaeid = ve.vaeid)
                            )
                    ), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrempenhado) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.ptres::text IN 
                                    (SELECT DISTINCT (vinculacaoestrategicapos.unicod::text || '.'::text) || vinculacaoestrategicapos.ptres::text
                                    FROM planacomorc.vinculacaoestrategicapos
                                    WHERE vinculacaoestrategicapos.vaeid = ve.vaeid)
                            )
                    ), 0::numeric) AS empenhado, 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrpago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.acacod::text IN 
                                    (SELECT DISTINCT (vinculacaoestrategicaacoes.unicod::text || '.'::text) || vinculacaoestrategicaacoes.acacod::text
                                    FROM planacomorc.vinculacaoestrategicaacoes
                            WHERE vinculacaoestrategicaacoes.vaeid = ve.vaeid))), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrpago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || substr(siopexecucao.plicod::text, 2, 4) IN 
                                    (SELECT DISTINCT (vinculacaoestrategicasubacoes.unicod::text || '.'::text) || vinculacaoestrategicasubacoes.sbacod::text
                                    FROM planacomorc.vinculacaoestrategicasubacoes
                                    WHERE vinculacaoestrategicasubacoes.vaeid = ve.vaeid))), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrpago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.plicod::text IN 
                                    (SELECT DISTINCT (vinculcaoestrategicapis.unicod::text || '.'::text) || vinculcaoestrategicapis.plicod::text
                                    FROM planacomorc.vinculcaoestrategicapis
                                    WHERE vinculcaoestrategicapis.vaeid = ve.vaeid))), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrpago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.ptres::text IN 
                                    (SELECT DISTINCT (vinculacaoestrategicapos.unicod::text || '.'::text) || vinculacaoestrategicapos.ptres::text
                                    FROM planacomorc.vinculacaoestrategicapos
                                    WHERE vinculacaoestrategicapos.vaeid = ve.vaeid))), 0::numeric) AS pago, 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrrapnaoprocessadopago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.acacod::text IN 
                                    (SELECT DISTINCT (vinculacaoestrategicaacoes.unicod::text || '.'::text) || vinculacaoestrategicaacoes.acacod::text
                                    FROM planacomorc.vinculacaoestrategicaacoes
                                    WHERE vinculacaoestrategicaacoes.vaeid = ve.vaeid))), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrrapnaoprocessadopago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || substr(siopexecucao.plicod::text, 2, 4) IN 
                                    (SELECT DISTINCT (vinculacaoestrategicasubacoes.unicod::text || '.'::text) || vinculacaoestrategicasubacoes.sbacod::text
                                    FROM planacomorc.vinculacaoestrategicasubacoes
                                    WHERE vinculacaoestrategicasubacoes.vaeid = ve.vaeid))), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrrapnaoprocessadopago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.plicod::text IN 
                                    (SELECT DISTINCT (vinculcaoestrategicapis.unicod::text || '.'::text) || vinculcaoestrategicapis.plicod::text
                                    FROM planacomorc.vinculcaoestrategicapis
                                    WHERE vinculcaoestrategicapis.vaeid = ve.vaeid))), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrrapnaoprocessadopago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.ptres::text IN 
                                    (SELECT DISTINCT (vinculacaoestrategicapos.unicod::text || '.'::text) || vinculacaoestrategicapos.ptres::text
                                    FROM planacomorc.vinculacaoestrategicapos
                                    WHERE vinculacaoestrategicapos.vaeid = ve.vaeid))), 0::numeric) AS rap_npp, 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrrapprocessadopago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.acacod::text IN 
                                    (SELECT DISTINCT (vinculacaoestrategicaacoes.unicod::text || '.'::text) || vinculacaoestrategicaacoes.acacod::text
                                    FROM planacomorc.vinculacaoestrategicaacoes
                                    WHERE vinculacaoestrategicaacoes.vaeid = ve.vaeid))), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrrapprocessadopago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || substr(siopexecucao.plicod::text, 2, 4) IN 
                                    (SELECT DISTINCT (vinculacaoestrategicasubacoes.unicod::text || '.'::text) || vinculacaoestrategicasubacoes.sbacod::text
                                    FROM planacomorc.vinculacaoestrategicasubacoes
                                    WHERE vinculacaoestrategicasubacoes.vaeid = ve.vaeid))), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrrapprocessadopago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.plicod::text IN 
                                    (SELECT DISTINCT (vinculcaoestrategicapis.unicod::text || '.'::text) || vinculcaoestrategicapis.plicod::text
                                    FROM planacomorc.vinculcaoestrategicapis
                                    WHERE vinculcaoestrategicapis.vaeid = ve.vaeid))), 0::numeric) + 
            COALESCE(
                    (SELECT sum(siopexecucao.vlrrapprocessadopago) AS sum
                    FROM spo.siopexecucao
                    WHERE siopexecucao.exercicio::integer = ve.exercicio 
                            AND ((siopexecucao.unicod::text || '.'::text) || siopexecucao.ptres::text IN 
                                    (SELECT DISTINCT (vinculacaoestrategicapos.unicod::text || '.'::text) || vinculacaoestrategicapos.ptres::text
                                    FROM planacomorc.vinculacaoestrategicapos
                                    WHERE vinculacaoestrategicapos.vaeid = ve.vaeid))), 0::numeric) AS rap_pp
        FROM planacomorc.vinculacaoacaoestrategicaexercicio ve
        JOIN planacomorc.vinculacaoacaoestrategica v USING (vacid)
        WHERE ve.vaeid = {$vaeid}
            AND ve.exercicio = {$exercicio};
DML;
    $data = $db->pegaLinha($sql);
    return $data;
}