<?php

/**
 * Modelo de Folha de Pagamento
 * @author Wesley Romualdo da Silva
 */
class FolhaPagamentoProfissionais extends Modelo {    
    
    /**
     * Nome da tabela especificada
     * @name $stNomeTabela
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "pisosalarial.folhapagamentoprofissionais";

    /**
     * Chave primaria.
     * @name $arChavePrimaria
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("fppid");

    /**
     * Atributos da Tabela
     * @name $arAtributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'fppid' => null,
        'carid' => null,
        'forid' => null,
        'vicid' => null,
        'sitid' => null,
        'sebid' => null,
        'lotid' => null,
        'etpid' => null,
        'fppcpf' => null,
        'fppnome' => null,
        'fppcargahoraria' => null,
        'fppzona' => null,
        'fppsalbase' => null,
        'fppadigrat' => null,
        'fpporgao' => null,
        'fppdesclotacao' => null,
        'entcodent' => null,
        'flpid' => null,
        'fppcarga' => null
    );
    var $arValue = array();
    var $arErro = array();
    var $totalRowLidas = 0;
    var $totalRowDescartadas = 0;
    var $arRowDescartadas = array();
    var $arqid = '';
    var $flpid = '';

    /**
     * Array para armazenar dados da função importarArquivoExcel().
     * @var array
     */
    private $_arError = array();
    private $_cargosBd = array();
    private $_formacoesBd = array();
    private $_vinculosBd = array();
    private $_situacoesBd = array();
    private $_lotacoesBd = array();
    private $_descLotacaoBd = array();
    private $_etapasBd = array();
    private $_segmentosBd = array();
    private $_clsPf = null;

    /**
     * Lista os log de upload do arquivo
     * @name listaLogArquivo
     * @author Wesley Romualdo da Silva
     * @access public
     * @return void
     */
    public function listaLogArquivo($arqid) {
        $sql = "SELECT arptotallinhalidas, arptotallinhadescartada FROM pisosalarial.arquivopagamento WHERE arqid = " . $arqid;
        $cabecalho = array("Total de Linhas Lidas", "Total de Linhas Descartadas");
        $this->monta_lista($sql, $cabecalho, 100, 50, false, "center");
        
        $sql = "SELECT detnumlinha, detvalor, detcoluna, detmsgerro FROM pisosalarial.detalhearquivopagamento WHERE arpid = (SELECT arpid FROM pisosalarial.arquivopagamento WHERE arqid = {$arqid}) ORDER BY detnumlinha ASC";
        $cabecalho = array("Linha", "Valor", "Nome da Coluna", "Menssagem");
        $this->monta_lista($sql, $cabecalho, 100, 50, false, "center");
    }

    /**
     * Lista dados profissionais
     * @name listaDadosProfissionais
     * @author Wesley Romualdo da Silva
     * @access public
     * @param int $ - Id do registro a ser pesquisado
     * @return void
     */
    public function listaDadosProfissionais($post, $regPorPag = 10) {

        $stWhere = "";
        $stWhere2 = "";
        $params = "";

        extract($post);

        if (isset($_REQUEST['ano']) && isset($_REQUEST['mes'])) {
            $params = "&mes=" . sprintf('%02s', $_REQUEST['mes']) . "&ano=" . $_REQUEST['ano'];
        }

        if (!empty($mesref)) {
            $stWhere .= " AND f.flpmesreferencia = '{$mesref}' ";
        }
        if (!empty($anoref)) {
            $stWhere .= " AND f.flpanoreferencia = '{$anoref}' ";
        }
        if (!empty($fppcpf)) {
            $cpf = str_replace(array('.', '-'), '', $fppcpf);
            $stWhere2 .= " AND fp.fppcpf = '{$cpf}' ";
        }
        if (!empty($fppnome)) {
            $stWhere2 .= " AND fp.fppnome iLike '%{$fppnome}%' ";
        }
        if (!empty($fppcargo)) {
            $stWhere2 .= " AND fp.carid = {$fppcargo} ";
        }

        $esdid = pegarEstadoAtual($_SESSION['piso']['muncod']);

        $stCampo = "";
        $stCamposEdit = "ca.carcodigo||' - '||ca.cardsc,
                         fppcargahoraria,
                         sitdsc,
                         fp.fppsalbase,
                         fp.fppadigrat,
                         fp.fppsalbase+fp.fppadigrat as remuneracao";

        if (possuiPerfil(array(PERFIL_CADASTRO_MUNICIPAL)) && !in_array($esdid, array(WF_APROVADO))) {

            $stCampo = "CASE WHEN f.flpfinalizado <> 't' THEN
                         '<center>
                          <a href=\"javascript:void(0)\" id=\"'|| fp.fppid ||'\" class=\"btEditar\"><img src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\" title=\"Alterar\"></a>
                          <a href=\"javascript:void(0)\" id=\"'|| fp.fppid ||'\" class=\"btExcluir\"><img src=\"/imagens/excluir.gif\" border=0 alt=\"Ir\" title=\"Excluir\"></a>
                          <input type=\"hidden\" name=\"fppid[]\" value=\"' || fp.fppid || '\" />
                         </center>'
                        ELSE '<a href=\"javascript:void(0)\" id=\"'|| fp.fppid ||'\" class=\"btVisualizar\"><img src=\"/imagens/consultar.gif \" border=0 alt=\"Ir\" title=\"Visualizar\"></a>'
                        END AS acao,";

            $sql = "SELECT
                        flpfinalizado
                    FROM pisosalarial.folhapagamento f
                    INNER JOIN pisosalarial.pisomunicipio pi ON pi.pmuid = f.pmuid
                    WHERE pi.muncod = '{$_SESSION['piso']['muncod']}'
                    {$stWhere}";

            $boFinalizado = $this->pegaUm($sql);

            if ($boFinalizado == 'f') {

                $sqlcargo = "SELECT carid as codigo, cardsc as descricao FROM pisosalarial.cargo WHERE carstatus = 'A' ORDER BY cardsc";
                $sqlsituacao = "SELECT sitid as codigo, sitdsc as descricao FROM pisosalarial.situacao WHERE sitstatus = 'A' ORDER BY sitdsc";

                $stCamposEdit = "'" . str_replace("'", '"', $this->monta_combo("carid[]", $sqlcargo, 'S', 'Selecione...', '', '', '', '200', 'N', '', true, '', 'Cargo')) . "<input type=\"hidden\" name=\"carid_temp[]\" value=\"' || fp.carid || '\">' as cargo,
                             '<input type=\"text\" id=\"fppcargahoraria_' || fp.fppid || '\" name=\"fppcargahoraria[]\" value=\"' || fp.fppcargahoraria || '\" class=\"normal\" size=\"8\">' as carga_horaria,
                             '" . str_replace("'", '"', $this->monta_combo("sitid[]", $sqlsituacao, 'S', 'Selecione...', '', '', '', '200', 'N', '', true, '', 'Situação')) . "<input type=\"hidden\" name=\"sitid_temp[]\" value=\"' || fp.sitid || '\">' as situacao,
                             '<input type=\"text\" value=\"' || REPLACE(CAST(fppsalbase AS VARCHAR(10)), '.', ',') || '\" id=\"fppsalbase_' || fp.fppid || '\" name=\"fppsalbase[]\" class=\"monetario normal\" onkeyup=\"this.value=mascaraglobal(&quot;[###.]###,##&quot;,this.value)\" size=\"16\"/>' as fppsalbase,
                             '<input type=\"text\" value=\"' || REPLACE(CAST(fppadigrat AS VARCHAR(10)), '.', ',') || '\" id=\"fppadigrat_' || fp.fppid || '\" name=\"fppadigrat[]\" class=\"monetario normal\" onkeyup=\"this.value=mascaraglobal(&quot;[###.]###,##&quot;,this.value)\" size=\"16\"/>' as fppadigrat,
                             '<input type=\"text\" value=\"' || REPLACE(CAST(fppadigrat+fppsalbase AS VARCHAR(10)), '.', ',') || '\" id=\"fppcalculo_' || fp.fppid || '\" name=\"fppcalculo[]\" disabled=\"disabled\" class=\"normal\" size=\"16\"/>' as fppcalculo";
            }
        } else {

            $stCampo = "'<a href=\"javascript:void(0)\" id=\"'|| fp.fppid ||'\" class=\"btVisualizar\"><img src=\"/imagens/consultar.gif \" border=0 alt=\"Ir\" title=\"Visualizar\"></a>' as acao,";
        }

        $sql = "SELECT
                     {$stCampo}
                     substr(fp.fppcpf, 0, 4)||'.'||substr(fp.fppcpf, 4, 3)||'.'||substr(fp.fppcpf, 7, 3)||'-'||substr(fp.fppcpf, 10, 2) as cpf,
                     fp.fppnome,
                     {$stCamposEdit}
				FROM {$this->stNomeTabela} fp
			    INNER JOIN pisosalarial.cargo            ca ON ca.carid = fp.carid AND ca.carstatus = 'A'
			    INNER JOIN pisosalarial.situacao         si ON si.sitid = fp.sitid AND si.sitstatus = 'A'
			    INNER JOIN pisosalarial.folhapagamento   f  ON fp.flpid = f.flpid
			    INNER JOIN pisosalarial.pisomunicipio    pi ON pi.pmuid = f.pmuid
			    WHERE pi.muncod = '{$_SESSION['piso']['muncod']}'
				" . $stWhere . $stWhere2;

        $cabecalho = array("Ações", "CPF", "Nome", "Cargo", "Carga horária", "Situação", "Salário base", "Adicionais e gratificações", "Remuneração (Sal. Base + Adic)");
        $align = array('center', 'left', 'left', 'left', 'right', 'left', 'right', 'right', 'right');
        $this->monta_lista($sql, $cabecalho, $regPorPag, 10, 'S', '', '', '', '', $align);
    }

    public function listaDadosProfissionaisDuplicar($post) {

        extract($post);

        $stWhere = "";
        if (!empty($mesref)) {
            $stWhere .= " AND f.flpmesreferencia = '{$mesref}' ";
        }
        if (!empty($anoref)) {
            $stWhere .= " AND f.flpanoreferencia = '{$anoref}' ";
        }
        if (!empty($_REQUEST['fppcpf'])) {
            $cpf = str_replace(array('.', '-'), '', $_REQUEST['fppcpf']);
            $stWhere .= " AND fp.fppcpf = '{$cpf}' ";
        }
        if (!empty($_REQUEST['fppnome'])) {
            $stWhere .= " AND fp.fppnome iLike '%{$_REQUEST['fppnome']}%' ";
        }

        $sqlcargo = "SELECT carid as codigo, cardsc as descricao FROM pisosalarial.cargo WHERE carstatus = 'A' ORDER BY cardsc";
        $sqlsituacao = "SELECT sitid as codigo, sitdsc as descricao FROM pisosalarial.situacao WHERE sitstatus = 'A' ORDER BY sitdsc";

        $sql = "SELECT
                    '<center><a href=\"javascript:void(0)\" id=\"'|| fp.fppid ||'\"
                     class=\"excluir\"><img src=\"/imagens/excluir.gif\" border=0 alt=\"Ir\"
                     title=\"Excluir\"> </a>
                     <input type=\"hidden\" name=\"fppid[]\" value=\"' || fp.fppid || '\" /></center>' as acao,
                     substr(fp.fppcpf, 0, 4) || '.' || substr(fp.fppcpf, 4, 3) || '.' || substr(fp.fppcpf, 7, 3) || '-' || substr(fp.fppcpf, 10, 2) as cpf,
                     fp.fppnome,
                     '" . str_replace("'", '"', $this->monta_combo("carid[]", $sqlcargo, 'S', 'Selecione...', '', '', '', '200', 'N', '', true, '', 'Cargo')) . "<input type=\"hidden\" name=\"carid_temp[]\" value=\"' || fp.carid || '\">' as cargo,
                     '<input type=\"text\" id=\"fppcargahoraria_' || fp.fppid || '\" name=\"fppcargahoraria[]\" value=\"' || fp.fppcargahoraria || '\" class=\"normal\" size=\"8\">' as carga_horaria,
                     '" . str_replace("'", '"', $this->monta_combo("sitid[]", $sqlsituacao, 'S', 'Selecione...', '', '', '', '200', 'N', '', true, '', 'Situação')) . "<input type=\"hidden\" name=\"sitid_temp[]\" value=\"' || fp.sitid || '\">' as situacao,
                     '<input type=\"text\" id=\"fppsalbase_' || fp.fppid || '\" name=\"fppsalbase[]\" class=\"monetario normal\" onkeyup=\"this.value=mascaraglobal(&quot;[###.]###,##&quot;,this.value)\" size=\"16\"/>' as fppsalbase,
                     '<input type=\"text\" id=\"fppadigrat_' || fp.fppid || '\" name=\"fppadigrat[]\" class=\"monetario normal\" onkeyup=\"this.value=mascaraglobal(&quot;[###.]###,##&quot;,this.value)\" size=\"16\"/>' as fppadigrat,
                     '<input type=\"text\" id=\"fppcalculo_' || fp.fppid || '\" name=\"fppcalculo[]\" disabled=\"disabled\" class=\"normal\" size=\"16\"/>' as fppcalculo
                FROM {$this->stNomeTabela} fp
                INNER JOIN pisosalarial.cargo            ca ON ca.carid = fp.carid AND ca.carstatus = 'A'
                INNER JOIN pisosalarial.folhapagamento   f  ON fp.flpid = f.flpid
                INNER JOIN pisosalarial.pisomunicipio    pi ON pi.pmuid = f.pmuid
                WHERE pi.muncod IS NOT NULL
                {$stWhere}
                AND pi.muncod = '{$_SESSION['piso']['muncod']}'";

        $cabecalho = array("Ação", "CPF", "Nome", "Cargo", "Carga horária", "Situação", "Salário base", "Adicionais e gratificações", "Remuneração (Sal. Base + Adic)");
        $align = array('center', 'left', 'left', 'left', 'right', 'left', 'right', 'right', 'right');
        $this->monta_lista($sql, $cabecalho, 10, 10, false, '', '', '', '', $align);
    }

    private function checkCPF($cpf) {
        // Verifiva se o número digitado contém todos os digitos
        $cpf = str_pad(ereg_replace('[^0-9]', '', $cpf), 11, '0', STR_PAD_LEFT);
        
        // Verifica se nenhuma das sequências abaixo foi digitada, caso seja, retorna falso
        if (strlen($cpf) != 11 || $cpf == '' || $cpf == '11111111111' || $cpf == '22222222222' || $cpf == '33333333333' || $cpf == '44444444444' || $cpf == '55555555555' || $cpf == '66666666666' || $cpf == '77777777777' || $cpf == '88888888888' || $cpf == '') {
            return false;
        } else {   // Calcula os números para verificar se o CPF é verdadeiro
            for ($t = 9; $t < 11; $t++) {
                for ($d = 0, $c = 0; $c < $t; $c++) {
                    $d += $cpf{$c} * (($t + 1) - $c);
                }

                $d = ((10 * $d) % 11) % 10;

                if ($cpf{$c} != $d) {
                    return false;
                }
            }

            return true;
        }
    }

    /**
     * Adiciona valores no array de erros;
     * 
     * @param string $col
     * @param string $lin
     * @param string $val
     * @param string $msg 
     * 
     * @author Ruy Ferreira <ruy.ferreira@squadra.com.br>
     */
    private function addError($col = null, $lin = null, $val = null, $msg = null) {

        if ($this->_arError) {
            $n = count($this->_arError);
        } else {
            $n = 0;
        }

        $this->_arError[$n] = array('linha' => $lin, 'coluna' => "'" . $col . "'", 'valor' => "'" . $val . "'", 'msg' => "'" . $msg . "'");
    }

    /**
     *
     * @param type $arrayBd
     * @param type $value 
     */
    private function checkArray($arrayBd, $value, $checkCod = false) {
        
        if($checkCod){
            $valueForCheck = 'codigo';            
        }else{
            $valueForCheck = 'descricao';
        }
        
        foreach ($arrayBd as $valueBd) {
            if (strtolower(trim($valueBd[$valueForCheck])) == strtolower(trim($value))) {
                return $valueBd['codigo'];
            }
        }
        
        return null;
    }

    /**
     * 
     * @param type $col
     * @param type $lin
     * @param type $val
     * @param type $name
     * @param type $checkCod
     * @param type $required
     * @return int $id
     */
    private function checkLine($col, $lin, $val, $name, $array, $checkCod = false, $required = true) {
        
        if (empty($val)) {
            if($required){
                $msg = $name . ' não preenchido.';
                $this->addError($col, $lin, $val, $msg);
            }
            
            return null;
        } else {
            $id = $this->checkArray($array, $val, $checkCod);
            if ($id) {
                return $id;
            } else {
                if($required){
                    $msg = $name . ' inválido.';
                    $this->addError($col, $lin, $val, $msg);
                }
                return null;
            }
        }
    }

    private function saveCol($arrayValues) 
    {
        
        $values = "";
        $n = count($arrayValues);
        for ($i = 0; $i < $n; $i++) {
            $values .= "'" . $arrayValues[$i] . "'";
            if ($i < $n - 1) {
                $values .= " , ";
            }
        }

        $sql = "INSERT INTO {$this->stNomeTabela}( 
                                                    fppcpf, 
                                                    fppnome, 
                                                    carid, 
                                                    caroutros, 
                                                    forid, 
                                                    vicid, 
                                                    sitid,
                                                    fppcargahoraria, 
                                                    lotid, 
                                                    entcodent, 
                                                    etpid, 
                                                    fppzona, 
                                                    sebid, 
                                                    fppsalbase, 
                                                    fppadigrat, 
                                                    flpid, 
                                                    fppcarga
                                                )
    				VALUES (" . $values . " , {$this->flpid}, now())";

        $this->executar($sql);
        $this->commit();
    }

    private function checkCol($arValues, $arNomeCol, $line) {
        $A = str_replace(array('.', ',', '-', ' '), '', $arValues['A']);
        $A = trim($A);
        $B = explode('-', $arValues['B']);
        
        if($B[2]){
            $B = $B[1] .'-' . $B[2];
        }else{
            $B = $B[1];
        }
        
        $C = trim($arValues['C']);
        $D = $arValues['D'];
        $E = $arValues['E'];
        $F = $arValues['F'];
        $G = $arValues['G'];
        $H = $arValues['H'];
        $I = $arValues['I'];
        $J = $arValues['J'];
        $K = $arValues['K'];
        $L = $arValues['L'];
        $M = $arValues['M'];
        $N = $arValues['N'];
        
        #Validções.
        #CPF
        if (empty($A)) {
            $this->addError($arNomeCol['A'], $line, $A, 'Cpf não preenchido.');
        } else {
            
            if ($this->checkCPF($A) === false) {
                                
                $this->addError($arNomeCol['A'], $line, $A, 'Cpf inválido.');
                $A = null;
            } else {
                $dados = $this->_clsPf->solicitarDadosResumidoPessoaFisicaPorCpf(trim($A));
                
                $obj = (array) simplexml_load_string($dados);
                
                if (!$obj['PESSOA']) {
                    $this->addError($arNomeCol['A'], $line, $A, 'Cpf não consta na base de dados.');
                } else {
                    $sql = "SELECT
                               fppid
                            FROM {$this->stNomeTabela} p
                            INNER JOIN pisosalarial.folhapagamento f ON f.flpid = p.flpid
                            INNER JOIN pisosalarial.pisomunicipio  pi ON pi.pmuid = f.pmuid
                            WHERE p.fppcpf = '$A' 
                            AND p.flpid = {$this->flpid}";

                    $boExiste = $this->pegaUm($sql);
                    if ($boExiste) {
                        $this->addError($arNomeCol['A'], $line, $A, 'CPF já existe na folha de pagamento');
                        $A = null;
                    } else {
                        $dados = (array) $obj['PESSOA'];
                        $name = $dados['no_pessoa_rf'];
                    }
                }
            }
        }

        #CARGO
        $B = $this->checkLine($arNomeCol['B'], $line, $B, 'Cargo', $this->_cargosBd);

        #DESCRIÇÃO DO CARGO
        
        if($B){
            if (strpos($B, 'Outros') && empty($C)) {
                $this->addError($arNomeCol['C'], $line, $C, 'Descrição do cargo não informado.');
            }
        }

        #FORMAÇÃO
        $D = $this->checkLine($arNomeCol['D'], $line, $D, 'Formação', $this->_formacoesBd);

        #VÍNCULO
        $E = $this->checkLine($arNomeCol['E'], $line, $E, 'Vinculo', $this->_vinculosBd);

        #SITUAÇÃO
        $F = $this->checkLine($arNomeCol['F'], $line, $F, 'Situação', $this->_situacoesBd);

        #CARGA HORÁRIA
        if (empty($G)) {
            $this->addError($arNomeCol['G'], $line, $G, 'Carga horária não informada.');
        } elseif (!is_int($G) || strlen($G) > 2) {
            $this->addError($arNomeCol['G'], $line, $G, 'Carga horária inválida.');
            $G = null;
        }

        #TIPO DE LOTAÇÃO
        $H = $this->checkLine($arNomeCol['H'], $line, $H, 'Lotação', $this->_lotacoesBd);

        #DESCRIÇÃO DA LOTAÇÃO
        $descLotRequired = (strtolower(trim($arValues['H'])) == 'escola do município')? true : false ;
        $I = $this->checkLine($arNomeCol['I'], $line, $I, 'Lotação', $this->_descLotacaoBd, true, $descLotRequired);
        
        if (strpos($arValues['B'], 'Professor') && strtolower(trim($arValues['H'])) == 'escola do município') {
            
            #ETAPA
            $J = $this->checkLine($arNomeCol['J'], $line, $J, 'Etapa', $this->_etapasBd);

            #ZONA
            if (empty($K)) {
                $this->addError($arNomeCol['K'], $line, $K, 'Zona não informada.');
            } else {
                if ($K == 'Urbana')
                    $K = "ZU";
                elseif ($K == 'Rural')
                    $K = "ZR";
                else {
                    $this->addError($arNomeCol['K'], $line, $K, 'Zona inválida.');
                    $K = null;
                }
            }

            #SEGMENTO DA EDUCAÇÃO BÁSICA 
            $L = $this->checkLine($arNomeCol['L'], $line, $L, 'Segmento', $this->_segmentosBd);
            
        } else {
            
            #ETAPA
            $J = $this->checkLine($arNomeCol['J'], $line, $J, 'Etapa', $this->_etapasBd, false, false);

            #ZONA
            if (!empty($K)) {
                if ($K == 'Urbana')
                    $K = "ZU";
                elseif ($K == 'Rural')
                    $K = "ZR";
                else {
                    $K = null;
                }
            }

            #SEGMENTO DA EDUCAÇÃO BÁSICA 
            $L = $this->checkLine($arNomeCol['L'], $line, $L, 'Segmento', $this->_segmentosBd, false, false);
        }

        #SALÁRIO BASE (R$)
        if (empty($M)) {
            $this->addError($arNomeCol['M'], $line, $M, 'Salário base não informado.');
        } else {
            $M = str_replace(array('R$', 'R', '$'), '', $M);
            $M = str_replace(',', '', $M);
            if (!is_numeric($M)) {
                $this->addError($arNomeCol['M'], $line, $M, 'Salário base inválido.');
            }
        }

        #ADICIONAIS E GRATIFICAÇÕES (R$)
        if (!empty($N)) {
            $N = str_replace(array('R$', 'R', '$'), '', $N);
            $N = str_replace(',', '', $N);
            if (!is_numeric($N)) {
                $this->addError($arNomeCol['N'], $line, $N, 'Salário base inválido.');
            }
        }
        $save = true;
        
        foreach($this->_arError as $value){
            if(!empty($value['linha']) && $value['linha'] == $line) $save = false;
        }
        
        if ($save) {
            $this->saveCol(array($A, $name, $B, $C, $D, $E, $F, $G, $H, $I, $J, $K, $L, $M, $N));
            
            $this->totalRowLidas = $this->totalRowLidas + 1;
        } else {
            $this->arRowDescartadas = $this->_arError;
            $this->totalRowDescartadas += 1;
        }
    }

    /**
     * Insere Log do processo.
     */
    public function insertLogProcessing() {
        $sql = "UPDATE pisosalarial.arquivopagamento SET
				  arptotallinhalidas = " . $this->totalRowLidas . ",
				  arptotallinhadescartada = " . $this->totalRowDescartadas . "
				WHERE
				  arqid = " . $this->arqid;
        $this->executar($sql);

        if (!empty($this->arRowDescartadas)) {

            foreach ($this->_arError as $value) {
                $sql = "INSERT INTO pisosalarial.detalhearquivopagamento ( 
                                                    detnumlinha,
                                                    detcoluna,
                                                    detvalor,
                                                    detmsgerro,
                                                    arpid
                                                )
    				VALUES ( " . implode(',', $value) . " , (SELECT arpid FROM pisosalarial.arquivopagamento WHERE arqid = {$this->arqid}))";

                $this->executar($sql);
                $this->commit();

                $this->_arError = array();
            }
        }
    }

    /**
     * Carrega as variaveis da class com seus respectivos valores vindas do banco de dados.
     */
    private function gear() {
        #Pega cargos.
        $sql = "SELECT carid as codigo, cardsc as descricao FROM pisosalarial.cargo WHERE carstatus = 'A' ORDER BY cardsc";
        $this->_cargosBd = $this->carregar($sql);

        #Pega formações.
        $sql = "SELECT forid as codigo, fordsc as descricao FROM pisosalarial.formacao WHERE forstatus = 'A'";
        $this->_formacoesBd = $this->carregar($sql);

        #Pega vinculo.
        $sql = "SELECT vicid as codigo, vicdsc as descricao FROM pisosalarial.vinculo WHERE vicstatus = 'A' ORDER BY vicdsc";
        $this->_vinculosBd = $this->carregar($sql);

        #Pega situacoes.
        $sql = "SELECT sitid as codigo, sitdsc as descricao FROM pisosalarial.situacao WHERE sitstatus = 'A' ORDER BY sitdsc";
        $this->_situacoesBd = $this->carregar($sql);

        #Pega lotação.
        $sql = "SELECT lotid as codigo, lotdsc as descricao FROM pisosalarial.lotacao WHERE lotstatus = 'A' ORDER BY lotdsc";
        $this->_lotacoesBd = $this->carregar($sql);

        #Pega etapa.
        $sql = "SELECT etpid as codigo, etpdsc as descricao FROM pisosalarial.etapa WHERE etpstatus = 'A'";
        $this->_etapasBd = $this->carregar($sql);

        #Pega segmento.
        $sql = "SELECT sebid as codigo, sebdsc as descricao FROM pisosalarial.segedubasica WHERE sebstatus = 'A'";
        $this->_segmentosBd = $this->carregar($sql);
        
        #Pega descrição da lota.
        $sql = "SELECT
                            e.entcodent as codigo,
                            entnome as descricao
                        FROM entidade.entidade e
                        INNER JOIN entidade.endereco ed ON ed.entid = e.entid
                        INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid
                        INNER JOIN entidade.funcao fu ON fu.funid = f.funid
                        WHERE e.entstatus = 'A'
                        AND e.entcodent IS NOT NULL
                        AND f.funid = 3
                        AND fu.funstatus = 'A'
                        AND ed.muncod = '{$_SESSION['piso']['muncod']}'
                        AND e.tpcid = 3
                        ORDER BY entnome";
        $this->_descLotacaoBd = $this->carregar($sql);
    }

    public function importarArquivoExcel() {
    
        $campos = array("arpdsc" => "'Coleta de Dados - Folha de Pagamento'");
        $file = new FilesSimec("arquivopagamento", $campos, "pisosalarial");
        $arquivoSalvo = $file->setUpload();
        $this->arqid = $file->getIdArquivo();

        $obFolhaPagamento = new FolhaPagamento();
        $post = array('muncod' => $_SESSION['piso']['muncod'],
            'flpanoreferencia' => $_REQUEST['anoref'],
            'flpmesreferencia' => $_REQUEST['mesref']);

        $flpid = $obFolhaPagamento->recuperarFolhaPagamento($post, true);

        if (!$flpid) {

            $this->flpid = $obFolhaPagamento->gerarFolhaPagamento($post);
        } else {
            $this->flpid = $flpid;
        }
        
        if ($arquivoSalvo) {
            $arquivo = $this->diretorioArquivo();
            $data = new Spreadsheet_Excel_Reader($arquivo, true, 'UTF-8');

            unlink($arquivo);

            // Le os registros da planilha
            $num = $data->rowcount($sheet_index = 0) + 1;

            $nomeCol = array
                (
                'A' => $data->val(2, 'A'),
                'B' => $data->val(2, 'B'),
                'C' => $data->val(2, 'C'),
                'D' => $data->val(2, 'D'),
                'E' => $data->val(2, 'E'),
                'F' => $data->val(2, 'F'),
                'G' => $data->val(2, 'G'),
                'H' => $data->val(3, 'H'),
                'I' => $data->val(3, 'I'),
                'J' => $data->val(2, 'J'),
                'K' => $data->val(2, 'K'),
                'L' => $data->val(2, 'L'),
                'M' => $data->val(2, 'M'),
                'N' => $data->val(2, 'N')
            );

            $msg = "O excel não está no formato correto.";

            #Validando campos no excel.
            if ($nomeCol['A'] != 'CPF') {
                echo "<script> alert('$msg É necessário o CPF na primeira coluna!'); </script>";
            } elseif ($nomeCol['N'] != 'ADICIONAIS E GRATIFICAÇÕES (R$)') {
                echo "<script> alert('$msg A ultima coluna do excel não é válida!'); </script>";
            } else {

                #Ativa as variaveis com seus respectivos valores do banco de dados.
                $this->gear();
                
                include APPRAIZ . "www/includes/webservice/cpf.php";
                $this->_clsPf = new PessoaFisicaClient("http://ws.mec.gov.br/PessoaFisica/wsdl");
                
                for ($i = 4; $i < $num; $i++) {

                    #Definindo os valores da planilha
                    $lineValues = array(
                        'A' => $data->val($i, 'A'),
                        'B' => $data->val($i, 'B'),
                        'C' => $data->val($i, 'C'),
                        'D' => $data->val($i, 'D'),
                        'E' => $data->val($i, 'E'),
                        'F' => $data->val($i, 'F'),
                        'G' => $data->val($i, 'G'),
                        'H' => $data->val($i, 'H'),
                        'I' => $data->val($i, 'I'),
                        'J' => $data->val($i, 'J'),
                        'K' => $data->val($i, 'K'),
                        'L' => $data->val($i, 'L'),
                        'M' => $data->val($i, 'M'),
                        'N' => $data->val($i, 'N')
                    );
                    if (
                            empty($lineValues['A']) &&
                            empty($lineValues['B']) &&
                            empty($lineValues['C']) &&
                            empty($lineValues['D']) &&
                            empty($lineValues['E']) &&
                            empty($lineValues['F']) &&
                            empty($lineValues['G']) &&
                            empty($lineValues['H']) &&
                            empty($lineValues['I']) &&
                            empty($lineValues['J']) &&
                            empty($lineValues['K']) &&
                            empty($lineValues['L']) &&
                            empty($lineValues['M']) &&
                            empty($lineValues['N'])
                    ) {
                        
                    } else {
                        $this->checkCol($lineValues, $nomeCol, $i);
                    }

                    //echo $A . ' - ' . $B . ' - ' . $G . ' - ' . $D . ' - ' . $H . ' - ' . $L . ' - ' . $M . '<br>';
                }

                #Insere o log de upload
                $this->insertLogProcessing();

                if ($this->arqid && $this->flpid) {

                    $sql = "insert into
                        pisosalarial.folhapagamentoarquivo (arpid, flpid)
                        values ((SELECT arpid FROM pisosalarial.arquivopagamento WHERE arqid = {$this->arqid}), {$this->flpid})";


                    $this->executar($sql);
                }
            }
            if ($this->totalRowDescartadas == 0 && $this->totalRowLidas > 0) {

                $this->commit();

                echo "<script>
			alert('Operação realizada com sucesso.');
                        window.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/listaProfissionais&acao=A&mostralog=true&arqid={$arqid}&anoref={$_REQUEST['anoref']}&mesref={$_REQUEST['mesref']}';
		      </script>";
            } elseif ($this->totalRowLidas > 0){
                $arqid = $this->arqid;
                $this->commit();
                echo "<script>
                            alert('Arquivo importado. Ver log de erros.');
                            window.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/importarArquivo&acao=A&mostralog=true&arqid={$arqid}&anoref={$_REQUEST['anoref']}&mesref={$_REQUEST['mesref']}';
                    </script>";
            } else {
                //pisosalarial.php?modulo=principal/folhapagamento/importarArquivo&acao=A
                $arqid = $this->arqid;
                $this->excluirArquivo($this->arqid);
                echo "<script>
                            alert('O arquivo selecionado não será importado porque apresenta erros. Ver log de importação.');
                            window.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/importarArquivo&acao=A&mostralog=true&arqid={$arqid}&anoref={$_REQUEST['anoref']}&mesref={$_REQUEST['mesref']}';
                    </script>";
            }


            #dados da planilha 1
//            foreach ($data->sheets as $k => $cel) {
//                if ($k == 0) {
//                    if (is_array($cel['cells'])) {
//
//                        #carrega linhas
//                        foreach ($cel['cells'] as $row => $arRow) {
////                            echo '<pre>';
////                            var_dump($arRow[11]);
////                            die('A');
////                            if(isset($arRow[4]) && isset($arRow[3])){
////
////                                $sql = "SELECT carid FROM pisosalarial.cargo WHERE carcodigo = '".trim($arRow[3])."' and carstatus = 'A'";
////                                $value = $this->pegaUm( $sql );
////
////                                if($arRow[3] != 'CARGO'){
////                                    echo "entrou";
////                                    ver($arRow[3], $value, $arRow, d);
////                                }else{
////                                    echo "nï¿½o entrou";
////                                }
////
//////                                $arRow[4] = 'null';
//////                                ksort($arRow);
////
////
////                            }
//                            if (!isset($arRow[11]) && isset($arRow[2])) {
//                                $arRow[11] = 'null';
//                                ksort($arRow);
//                            }
//                            if (!isset($arRow[12]) && isset($arRow[2])) {
//                                $arRow[12] = 'null';
//                                ksort($arRow);
//                            }
//                            if (!isset($arRow[13]) && isset($arRow[2])) {
//                                $arRow[13] = 'null';
//                                ksort($arRow);
//                            }
//                            if (!isset($arRow[14]) && isset($arRow[2])) {
//                                $arRow[14] = 'null';
//                                ksort($arRow);
//                            }
//
//                            if (is_array($arRow) && $row > 3) {
//
//                                #carrega colunas
//                                foreach ($arRow as $col => $cell) {
//
//                                    if (!empty($cell)) {
//                                        if ($col < 16) {
//                                            $this->validaRegistroPlanilha($row, $col, $cell);
//                                        }
//                                    }
//                                }
//
//                                if (sizeof($this->arValue) > 0) {
//                                    #total de linhas processadas
//                                    $this->totalRowLidas++;
//                                    if (sizeof($this->arValue) == 15) {
//                                        #insere os registros valido no banco
//                                        $this->montaInsert();
//                                    } else {
//                                        if (!in_array($row, $this->arRowDescartadas)) {
//                                            $this->totalRowDescartadas++;
//                                            array_push($this->arRowDescartadas, $row);
//                                        }
//                                    }
//                                    $this->arValue = array();
//                                } else {
//                                    break;
//                                }
//                            }
//                        }
//                        if ($this->arqid && $this->flpid) {
//
//                            $sql = "insert into
//                                        pisosalarial.folhapagamentoarquivo
//                                        (arpid, flpid)
//                                    values
//                                        ((SELECT arpid FROM pisosalarial.arquivopagamento WHERE arqid = {$this->arqid}),
//                                        {$this->flpid})";
//
//
//                            $this->executar($sql);
//                        }
//                    }
//                } else {
//                    break;
//                }
//            }
//            if ($this->totalRowDescartadas > 0) {
//                $this->rollback();
//            }
//
//            #insere o log de upload
//            $this->insereLogProcessamento();
//
//            if ($this->totalRowDescartadas == 0 && $this->totalRowLidas > 0) {
//                //pisosalarial.php?modulo=principal/folhapagamento/listaProfissionais&acao=A
//                $this->commit();
////			    $this->sucesso('principal/folhapagamento/listaProfissionais', '&acao=A&mostralog=true&arqid='.$this->arqid.'&anoref='.$_REQUEST['anoref'].'&mesref='.$_REQUEST['mesref']);
//                echo "<script>
//			alert('Operação realizada com sucesso.');
//                        window.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/listaProfissionais&acao=A&mostralog=true&arqid={$arqid}&anoref={$_REQUEST['anoref']}&mesref={$_REQUEST['mesref']}';
//		      </script>";
//            } else {
//                //pisosalarial.php?modulo=principal/folhapagamento/importarArquivo&acao=A
//                $arqid = $this->arqid;
//                $this->excluirArquivo($this->arqid);
//                echo "<script>
//                            alert('O arquivo selecionado não será importado porque apresenta erros. Ver log de importação.');
//                            window.location.href = 'pisosalarial.php?modulo=principal/folhapagamento/importarArquivo&acao=A&mostralog=true&arqid={$arqid}&anoref={$_REQUEST['anoref']}&mesref={$_REQUEST['mesref']}';
//                    </script>";
//            }
        }
    }

    public function excluirArquivo($arqid, $boExFolha = false) {
        if (!empty($arqid)) {
            if ($boExFolha) {
                $sql = "DELETE FROM {$this->stNomeTabela} WHERE arpid = (SELECT arpid FROM pisosalarial.arquivopagamento WHERE arqid = $arqid) and flpmesano = '" . date('m/Y') . "'";
                $this->executar($sql);
            }
            //$sql = "DELETE FROM pisosalarial.arquivopagamento where arqid=" . $arqid;
            //$this->executar($sql);
            $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=" . $arqid;
            $this->executar($sql);

            $file = new FilesSimec();
            $file->excluiArquivoFisico($arqid);
        }
    }

    public function validaRegistroPlanilha($row, $col, $valor) {
        $valor = trim($valor);
        if (empty($valor) || $valor == 'null') {
            $value = $valor;
        } else if ($col == 1) {#coluna CPF
            if (validaCPF($valor)) {
                $value = "'" . str_replace(array('.', '-'), '', $valor) . "'";
            } else {
                $value = null;
            }
        } else if ($col == 3) { #Coluna Cargo
            $valor = substr($valor, 0, strpos($valor, '-'));
            $sql = "SELECT carid FROM pisosalarial.cargo WHERE carcodigo = '" . trim($valor) . "' and carstatus = 'A'";
            $value = $this->pegaUm($sql);
        } else if ($col == 5) { #Coluna Formaï¿½ï¿½o
            $sql = "SELECT forid FROM pisosalarial.formacao WHERE fordsc ilike '%" . trim($valor) . "%' and forstatus = 'A'";
            $value = $this->pegaUm($sql);
        } else if ($col == 6) { #Coluna Vinculo
            $sql = "SELECT vicid FROM pisosalarial.vinculo WHERE vicdsc ilike '%" . trim($valor) . "%' and vicstatus = 'A'";
            $value = $this->pegaUm($sql);
        } else if ($col == 7) { #Coluna Situacao
            $sql = "SELECT sitid FROM pisosalarial.situacao WHERE sitdsc ilike '%" . trim($valor) . "%' and sitstatus = 'A'";
            $value = $this->pegaUm($sql);
        } else if ($col == 9) { #Coluna Lotaï¿½ï¿½o
            $sql = "SELECT lotid FROM pisosalarial.lotacao WHERE lotdsc ilike '%" . trim($valor) . "%' and lotstatus = 'A'";
            $value = $this->pegaUm($sql);
        } else if ($col == 11) { #Coluna Etapa
            $sql = "SELECT etpid FROM pisosalarial.etapa WHERE etpdsc ilike '%" . trim($valor) . "%' and etpstatus = 'A'";
            $value = $this->pegaUm($sql);
        } else if ($col == 12) { #Coluna Zona
            if ($valor == 'Urbana')
                $value = "'ZU'";
            if ($valor == 'Rural')
                $value = "'ZR'";
        } else if ($col == 13) { #Coluna Segmento da Educaï¿½ï¿½o Bï¿½sica
            $sql = "SELECT sebid FROM pisosalarial.segedubasica WHERE sebdsc ilike '%" . trim($value) . "%' and sebstatus = 'A'";
            $value = $this->pegaUm($sql);
        } else if ($col == 14 || $col == 15 || $col == 16) {
            $value = str_replace(array('R$', 'R', '$'), '', $valor);
            if (strpos($value, ',')) {
                $value = str_replace(',', '', $value);
            }
        } else {
            $value = "'" . $valor . "'";
        }

        if (!empty($value)) {
            array_push($this->arValue, $value);
        }
    }

    public function montaInsert() {

        $sql = "SELECT
	               fppid
	            FROM {$this->stNomeTabela} p
	            INNER JOIN pisosalarial.folhapagamento f ON f.flpid = p.flpid
	            INNER JOIN pisosalarial.pisomunicipio  pi ON pi.pmuid = f.pmuid
	            WHERE p.fppcpf = {$this->arValue[0]}
	            AND f.flpanoreferencia = '{$_REQUEST['anoref']}'
	            AND f.flpmesreferencia = '{$_REQUEST['mesref']}'
	            AND pi.muncod = '{$_SESSION['piso']['muncod']}'";

        $boExiste = $this->pegaUm($sql);

        if (empty($boExiste)) {
            $sql = "INSERT INTO {$this->stNomeTabela}( fppcpf, fppnome, carid, caroutros, forid, vicid, sitid,
    							fppcargahoraria, lotid, fppdesclotacao, etpid, fppzona, sebid, fppsalbase, fppadigrat, flpid, fppcarga)
    				VALUES (" . implode(',', $this->arValue) . ", {$this->flpid}, now())";

            $this->executar($sql);
        }
    }

    public function diretorioArquivo() {
        $sql = "SELECT * FROM public.arquivo WHERE arqid = " . $this->arqid;
        $arquivo = $this->carregar($sql);
        if ($arquivo) {
            $arquivo = current($arquivo);
        } else {
            $erro = "Arquivo não encontrado.";
            return false;
        }
        $caminhoAtual = APPRAIZ . 'arquivos/pisosalarial/' . floor($this->arqid / 1000) . '/' . $this->arqid;
        $pastaTemp = APPRAIZ . 'arquivos/pisosalarial/' . floor($this->arqid / 1000) . '/tmp_';
        if (!is_dir($pastaTemp)) {
            mkdir($pastaTemp, 0777);
        }
        $pastaTemp = $pastaTemp . '/' . $this->arqid;
        $filename = $pastaTemp . '.' . $arquivo['arqextensao'];

        if (!file_exists($filename)) {
            copy($caminhoAtual, $pastaTemp);
            $this->renomeiaArquivo($pastaTemp, $filename);
        }
        return $filename;
    }

    public function renomeiaArquivo($arquivoFelho, $arquivoNovo) {
        rename($arquivoFelho, $arquivoNovo);
    }

    public function duplicarArquivoAnterior() {

        $stWhere = "";
        if (isset($_REQUEST['mes']) && isset($_REQUEST['ano'])) {
            if (sprintf('%02s', $_REQUEST['mes']) == '01') {
                $stWhere = "where flpmesano = '12/" . ($_REQUEST['ano'] - 1) . "'";
            } else {
                $stWhere = "where flpmesano = '" . sprintf('%02s', $_REQUEST['mes'] - 1) . "/" . $_REQUEST['ano'] . "'";
            }
            $stParams = "WHERE f.flpanoreferencia = '{$_REQUEST['ano']}' AND f.flpmesreferencia = '{$_REQUEST['mes']}' AND pi.muncod = '{$_SESSION['piso']['muncod']}'";
        }

        $sql = "SELECT
		          count(fppid)
		        FROM {$this->stNomeTabela} fp
		        INNER JOIN pisosalarial.folhapagamento f ON f.flpid = fp.flpid
		        INNER JOIN pisosalarial.pisomunicipio    pi ON pi.pmuid = f.pmuid
		        {$stParams}";

        $total = $this->pegaUm($sql);

        if ($total == 0) {
            $sql = "INSERT INTO {$this->stNomeTabela}( carid, forid, vicid, sitid, sebid, lotid, etpid, flpcpf, flpnome, flpcargahoraria,
	  						flpzona, flpsalbase, flpadigrat, flpmesano, flporgao, flpdesclotacao, arpid, flpuf, flpmunicipio)
					(SELECT carid, forid, vicid, sitid, sebid, lotid, etpid, flpcpf, flpnome, flpcargahoraria,
						flpzona, flpsalbase, flpadigrat, '" . $params . "', flporgao, flpdesclotacao, arpid, flpuf, flpmunicipio
					FROM {$this->stNomeTabela}
					{$stWhere})";

            $this->executar($sql);
            if ($this->commit()) {
                return 'true';
            } else {
                return 'false';
            }
        } else {
            return 'existe';
        }
    }

    public function carregaRegistroMes($post) {

        extract($post);

        $stWhere = "";
        if ($anoref) {
            $stWhere .= " AND f.flpanoreferencia = '{$anoref}' ";
        }
        if ($anoref) {
            $stWhere .= " AND f.flpmesreferencia = '{$mesref}' ";
        }

        $sql = "SELECT
		              fppid,
		              carid,
		              forid,
		              vicid,
		              sitid,
		              sebid,
		              lotid,
		              etpid,
		              fppcpf,
		              fppnome,
		              fppcargahoraria,
		              fppzona,
  					  fppsalbase,
  					  fppadigrat,
  					  fpporgao,
  					  fppdesclotacao
  				FROM {$this->stNomeTabela} p
  				INNER JOIN pisosalarial.folhapagamento f ON f.flpid = p.flpid
  				INNER JOIN pisosalarial.pisomunicipio  pi ON pi.pmuid = f.pmuid
  				where pi.muncod IS NOT NULL
		        and pi.muncod = '{$_SESSION['piso']['muncod']}'
		        {$stWhere}
		        order by fppnome asc";

        $arDados = $this->pegaLinha($sql);
        $arDados = $arDados ? $arDados : array();
        return $arDados;
    }

    public function salvarFolhaProfissional($post) {

        extract($post);

        $fppcpf = str_replace(array('.', '-'), '', $fppcpf);
        $fppadigrat = str_replace('.', '', $fppadigrat);
        $fppadigrat = str_replace(',', '.', $fppadigrat);
        $fppsalbase = str_replace('.', '', $fppsalbase);
        $fppsalbase = str_replace(',', '.', $fppsalbase);
        $fpporgao = (empty($fpporgao) ? 'null' : $fpporgao);
        $fppnome = trim($fppnome);
        $fppdesclotacao = trim($fppdesclotacao);

        $sql = "SELECT
		          fppid
		        FROM {$this->stNomeTabela} p
		        INNER JOIN pisosalarial.folhapagamento f ON f.flpid = p.flpid
		        INNER JOIN pisosalarial.pisomunicipio    pi ON pi.pmuid = f.pmuid
		        WHERE p.fppcpf = '" . $fppcpf . "'
		        AND f.flpanoreferencia = '{$anoref}'
		        AND f.flpmesreferencia = '{$mesref}'
		        AND pi.muncod = '{$_SESSION['piso']['muncod']}'
		        LIMIT 1";

        $boExiste = $this->pegaUm($sql);

        if (!$boExiste || !empty($fppid)) {
            if (empty($fppid)) {

                $sql = "INSERT INTO {$this->stNomeTabela}
				            (carid, forid, vicid, " . (!empty($sitid) ? 'sitid,' : '') . " " . (!empty($sebid) ? 'sebid,' : '') . " lotid, " . (!empty($etpid) ? 'etpid,' : '') . " fppcpf, fppnome, fppcargahoraria, fppzona, fppsalbase, fppadigrat, fpporgao, fppdesclotacao, flpid, entcodent, caroutros)
						VALUES
						    ($carid, $forid, $vicid, " . (!empty($sitid) ? $sitid . ',' : '') . " " . (!empty($sebid) ? $sebid . ',' : '') . " $lotid, " . (!empty($etpid) ? $etpid . ',' : '') . " '$fppcpf', '$fppnome', '$fppcargahoraria', '$fppzona', $fppsalbase, $fppadigrat, '$fpporgao', '$fppdesclotacao', '$flpid', '$entcodent', '$caroutros')";
            } else {
                $sql = "UPDATE {$this->stNomeTabela} SET
						  carid = $carid, forid = $forid,
						  vicid = $vicid,
						  " . (!empty($sitid) ? "sitid = $sitid," : '') . "
						  " . (!empty($sebid) ? "sebid = $sebid," : '') . "
						  lotid = $lotid,
						  " . (!empty($etpid) ? "etpid = $etpid," : '') . "
						  fppcpf = '$fppcpf',
						  fppnome = '$fppnome',
						  fppcargahoraria = '$fppcargahoraria',
						  fppzona = '$fppzona',
						  fppsalbase = '$fppsalbase',
						  fppadigrat = '$fppadigrat',
						  fpporgao = '$fpporgao',
						  fppdesclotacao = '$fppdesclotacao',
						  entcodent = '$entcodent',
						  caroutros = '$caroutros'
						WHERE
						  fppid = $fppid";
            }

            $this->executar($sql);
            if ($this->commit())
                return 'true';
            else
                return 'false';
        } else {
            return 'existe';
        }
    }

    public function excluirProfissionalFolhaPagamento($id) {
        $sql = "SELECT
	               p.fppid
	            FROM {$this->stNomeTabela} p
	            INNER JOIN pisosalarial.folhapagamento f ON f.flpid = p.flpid
	            WHERE p.fppid = {$id}
	            AND f.flpfinalizado = 't'";

        $boFinalizado = $this->pegaUm($sql);

        if ($boFinalizado) {
            return 'finalizado';
        } else {
            $sql = "DELETE FROM {$this->stNomeTabela} WHERE fppid = {$id}";
            $this->executar($sql);
            return 'true';
        }
    }

    public function recuperarDadosProfissionalPorId($id) {
        $sql = "SELECT
	               p.fppid,
	               flpid,
	               substr(p.fppcpf, 0, 4)||'.'||substr(p.fppcpf, 4, 3)||'.'||substr(p.fppcpf, 7, 3)||'-'||substr(p.fppcpf, 10, 2) as fppcpf,
	               p.fppnome,
	               c.cardsc,
	               f.fordsc,
	               v.vicdsc,
	               s.sitdsc,
	               p.fppcargahoraria,
	               l.lotdsc,
	               t.entcodent,
	               p.fppdesclotacao,
	               t.entnome,
	               e.etpdsc,
	               p.fppzona,
	               b.sebdsc,
	               p.fppsalbase,
	               p.fppadigrat
	            FROM {$this->stNomeTabela} p
	            INNER JOIN pisosalarial.cargo          c ON p.carid = c.carid
	            INNER JOIN pisosalarial.formacao       f ON p.forid = f.forid
	            INNER JOIN pisosalarial.vinculo        v ON p.vicid = v.vicid
	            INNER JOIN pisosalarial.situacao       s ON p.sitid = s.sitid
	            INNER JOIN pisosalarial.segedubasica   b ON p.sebid = b.sebid
	            INNER JOIN pisosalarial.lotacao        l ON p.lotid = l.lotid
	            INNER JOIN pisosalarial.etapa          e ON p.etpid = e.etpid
	            LEFT JOIN entidade.entidade            t ON p.entcodent = t.entcodent
	            WHERE p.fppid = {$id}";

        return $this->pegaLinha($sql);
    }

    public function duplicarMesFolhaPagamento($post) {
        $arWhere = array();

        if (is_array($post)) {
            extract($post);
        }

        if (!empty($mesdup)) {
            $arWhere[] = "f.flpmesreferencia = '{$mesdup}'";
        }
        if (!empty($anodup)) {
            $arWhere[] = "f.flpanoreferencia = '{$anodup}'";
        }

        $sql = "SELECT
                    flpid
                FROM pisosalarial.folhapagamento f
                INNER JOIN pisosalarial.pisomunicipio pi ON pi.pmuid = f.pmuid
                WHERE pi.muncod = '{$_SESSION['piso']['muncod']}'
                " . (count($arWhere) ? "AND " . implode(' AND ', $arWhere) : "");

        $flpid = $this->pegaUm($sql);

        if ($flpid && count($fppid)) {

            foreach ($fppid as $key => $value) {

                $sql = "SELECT
                            fppid
                        FROM pisosalarial.folhapagamentoprofissionais e
                        INNER JOIN pisosalarial.folhapagamento f ON f.flpid = e.flpid
                        INNER JOIN pisosalarial.pisomunicipio    pi ON pi.pmuid = f.pmuid
                        WHERE fppcpf = (SELECT
                                            fppcpf
                                        FROM pisosalarial.folhapagamentoprofissionais p
                                        WHERE fppid = {$value})
                        AND pi.muncod = '{$_SESSION['piso']['muncod']}'
                        " . (count($arWhere) ? "AND " . implode(' AND ', $arWhere) : "");

                $boExiste = $this->pegaUm($sql);

                if ($boExiste) {
                    $sql = "DELETE FROM pisosalarial.folhapagamentoprofissionais WHERE fppid = $boExiste";
                    $this->executar($sql);
                }

//                if(!$boExiste){
                $fppsalbase = formata_valor_sql($post['fppsalbase'][$value]);
                $fppadigrat = formata_valor_sql($post['fppadigrat'][$value]);

                $fppsalbase = $fppsalbase ? $fppsalbase : 0;
                $fppadigrat = $fppadigrat ? $fppadigrat : 0;

//                    if(!empty($fppsalbase) && !empty($fppadigrat)){

                $sql = "INSERT INTO {$this->stNomeTabela}
                                ( carid, forid, vicid, sitid, sebid, lotid, etpid, fppcpf, fppnome, fppcargahoraria, fppzona, fppsalbase, fppadigrat, fpporgao, fppdesclotacao, flpid, caroutros, entcodent)
                                (SELECT
                                    {$post['carid'][$value]}, forid, vicid, {$post['sitid'][$value]}, sebid, lotid, etpid, fppcpf, fppnome, {$post['fppcargahoraria'][$value]}, fppzona, {$fppsalbase}, {$fppadigrat}, fpporgao, fppdesclotacao, {$flpid},caroutros, entcodent
                                 FROM {$this->stNomeTabela}
                                 WHERE fppid = {$value})";

                $this->executar($sql);
//                    }
//                }
            }
            $this->commit();
        }
    }

    public function salvarProfissionais($post) {
        $arWhere = array();

        if (is_array($post)) {
            extract($post);
        }

        foreach ($fppid as $key => $value) {

            $fppsalbase = formata_valor_sql($post['fppsalbase'][$value]);
            $fppadigrat = formata_valor_sql($post['fppadigrat'][$value]);

            $fppsalbase = $fppsalbase ? $fppsalbase : 0;
            $fppadigrat = $fppadigrat ? $fppadigrat : 0;

            $sql = "UPDATE {$this->stNomeTabela}
                    SET carid = {$post['carid'][$value]},
                        sitid = {$post['sitid'][$value]},
                        fppcargahoraria = '{$post['fppcargahoraria'][$value]}',
                        fppsalbase = '{$fppsalbase}',
                        fppadigrat = '{$fppadigrat}'
                    WHERE fppid = {$value}";

            $this->executar($sql);
        }
        $this->commit();
    }

    public function recuperarEscolaCampoPopup($entcodent) {
        $sql = "SELECT
                    e.entcodent as value,
                    entnome as descricao
                FROM entidade.entidade e
                INNER JOIN entidade.endereco ed ON ed.entid = e.entid
                INNER JOIN entidade.funcaoentidade f ON f.entid = e.entid
                WHERE e.entstatus = 'A'
                AND e.entcodent IS NOT NULL
                AND f.funid = 3
                AND ed.muncod = '{$_SESSION['piso']['muncod']}'
                AND e.entcodent = '{$entcodent}'
                ORDER BY entnome";

        return $this->pegaLinha($sql);
    }

    public function recuperarPorId($id) {
        $sql = "SELECT
                   *
                FROM {$this->stNomeTabela} p
                LEFT JOIN pisosalarial.cargo          c ON p.carid = c.carid
                LEFT JOIN pisosalarial.formacao       f ON p.forid = f.forid
                LEFT JOIN pisosalarial.vinculo        v ON p.vicid = v.vicid
                LEFT JOIN pisosalarial.situacao       s ON p.sitid = s.sitid
                LEFT JOIN pisosalarial.segedubasica   b ON p.sebid = b.sebid
                LEFT JOIN pisosalarial.lotacao        l ON p.lotid = l.lotid
                LEFT JOIN pisosalarial.etapa          e ON p.etpid = e.etpid
                LEFT JOIN entidade.entidade            t ON p.entcodent = t.entcodent
                WHERE p.fppid = {$id}";

        return $this->pegaLinha($sql);
    }

    public function recuperarJornadaProfissionais($cargo) {
        $sql = "SELECT
                    sum(p.fppcargahoraria::numeric/40) as result
                FROM pisosalarial.tipo_cargo t
                INNER JOIN pisosalarial.cargo c ON c.tpcid = t.tpcid
                INNER JOIN pisosalarial.folhapagamentoprofissionais p ON p.carid = c.carid
                INNER JOIN pisosalarial.folhapagamento f ON p.flpid = f.flpid
                INNER JOIN pisosalarial.pisomunicipio m ON m.pmuid = f.pmuid
                WHERE t.tpcid = {$cargo}
                AND t.tpcstatus = 'A'
                AND c.carstatus = 'A'
                AND m.muncod = '{$_SESSION['piso']['muncod']}'";

        return $this->pegaUm($sql);
    }

    public function recuperarJornadaAluno($ano = '2010') {
        $sql = "SELECT
                    sum(((to_char((hr_final || ':' || hr_final_minuto || ':00')::time-(hr_inicial || ':' || hr_inicial_minuto || ':00')::time, 'HH')::numeric)+
                    (to_char((hr_final || ':' || hr_final_minuto || ':00')::time-(hr_inicial || ':' || hr_inicial_minuto || ':00')::time, 'MI')::numeric)/60)/26.66) as jornada
                FROM educacenso_{$ano}.tab_turma t
                INNER JOIN educacenso_{$ano}.tab_matricula m ON t.pk_cod_turma = m.fk_cod_turma
                INNER JOIN educacenso_{$ano}.tb_escola_inep_{$ano} i ON m.fk_cod_entidade = i.pk_cod_entidade
                WHERE i.co_municipio = '{$_SESSION['piso']['muncod']}'
                AND t.id_status = 1
                AND m.id_status = 1";

        return $this->pegaUm($sql);
    }

    public function recuperarAnosReferencia() {
        $sql = "SELECT
                    farano
                FROM pisosalarial.folhaanoreferencia
                WHERE farstatus = 'A'";

        return $this->carregar($sql);
    }

}

?>