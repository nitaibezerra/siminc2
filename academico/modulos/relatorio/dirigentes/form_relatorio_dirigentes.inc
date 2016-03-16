<?PHP
    function atualizaComboInstituicao( $dados ){
        global $db;

        $funid = $dados['funid'];

        if( $funid == 'S' ){
            $funid = ACA_ID_UNIVERSIDADE;
        }else{
            $funid = ACA_ID_ESCOLAS_TECNICAS;
        }

        $sql = "
            SELECT  e.entid AS codigo,
                    upper(e.entnome) AS descricao
            FROM entidade.entidade e2

            JOIN entidade.entidade e ON e2.entid = e.entid
            JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
            LEFT JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
            JOIN entidade.endereco ed ON ed.entid = e.entid
            JOIN territorios.municipio mun ON mun.muncod = ed.muncod

            WHERE ef.funid = {$funid}

            ORDER BY e.entnome
        ";
        $db->monta_combo('entid', $sql, 'S', "Digite o nome e selecione a Unidade...", '', '', '', 600, 'S', 'entid', '', $entid, 'Unidade', '', 'chosen-select');
        echo '<script>$1_11("#entid").chosen();</script>';
        die();
    }

    function atualizaComboCampus($dados) {
    	$sql = "SELECT  e.entid as codigo,
    					upper(e.entnome) as descricao
    			  FROM  entidade.entidade e2
    		INNER JOIN	entidade.entidade e ON e2.entid = e.entid
    		INNER JOIN	entidade.funcaoentidade ef ON ef.entid = e.entid
    		INNER JOIN	entidade.funentassoc ea ON ea.fueid = ef.fueid
    		 LEFT JOIN	entidade.endereco ed ON ed.entid = e.entid
    		 LEFT JOIN	territorios.municipio mun ON mun.muncod = ed.muncod
    			 WHERE	ea.entid = {$dados['entid']}
    		 	   AND	e.entstatus = 'A'
    		  ORDER BY	e.entnome";

    	combo_popup( "camp_id", $sql, "Campus", "400x400", 0, array(), "", "S", false, false, 5, 400 );
		die();
    }
    
    function atualizaComboTipoMembro( $dados ){
        global $db;

        $opc = $dados['opc'];

        if( $opc == 'U' ){
            $where = " WHERE t.tpmnivel = 'I' ";
        }else
            if( $opc == 'C' ){
                $where = " WHERE t.tpmnivel = 'C' ";
        }else{
            $where = "WHERE 1<>1 ";
        }

        $sql = "
            SELECT  t.tpmid AS codigo,
                    t.tpmdsc AS descricao

            FROM academico.tipomembro AS t
            {$where}
            ORDER BY t.tpmdsc
        ";
        $db->monta_combo('tpmid', $sql, 'S', "Digite o nome e selecione o Tipo de Membro", '', '', '', 600, 'N', 'tpmid', '', $tpmid, 'Tipo de Membro', '', 'chosen-select');
        echo '<script>$1_11("#tpmid").chosen();</script>';
        die();
    }

    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    if ($_POST['colunas']){
        ini_set("memory_limit","256M");
        include("result_relatorio_dirigentes.inc");
        exit;
    }

    include APPRAIZ . 'includes/Agrupador.php';
    include APPRAIZ . 'includes/cabecalho.inc';
    print '<br/>';

    monta_titulo( 'Rede Federal - Dirigentes', 'Relatório - Dirigentes/Intelocutor/Membro do Conselho' );
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
    <title>Relatório - Avaliação Final</title>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

    <script type="text/javascript">
        function atualizaComboInstituicao( funid ) {
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=atualizaComboInstituicao&funid=" + funid,
                async: false,
                success: function (resp) {
                    $('#td_combo_instituicao').html(resp);
                    $1_11('#entid').change(atualizaUnidadeCampus);
                }
            });
        }

        function atualizaComboCampus( entid ) {
            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=atualizaComboCampus&entid=" + entid,
                async: false,
                success: function (resp) {
                    $('#td_combo_campus').html(resp);
                }
            });
        }

        function atualizaUnidadeCampus( entid ) {
        	if( $1_11('#entid').val() ){
        		$1_11('#unid_camp_C').prop('disabled', false);
        	} else {
        		$1_11('#unid_camp_C').prop('disabled', true);
        	}
        }

        function atualizaOpcDirigentes( opc ){
            if( opc == 'U' ){
                $('#s_funid_R').css('display', '');
                $('#s_funid_D').css('display', 'none');
                $('#popup_camp').css('display', 'none');
                $('input:radio[name=funid]').attr('checked', false);
                $($('#s_funid_R input:radio[name=funid]')[0]).attr('checked', true);
                
                $('#colunas option[value=nome_campus]').remove();
                $('#naoColunas option[value=nome_campus]').remove();
                $('#naoColunas').append( $("<option/>", {value:'nome_campus', text: '08. Campus'}) );
            }else{
                $('#s_funid_R').css('display', 'none');
                $('#s_funid_D').css('display', '');
            	$('#popup_camp').css('display', '');
                $('input:radio[name=funid]').attr('checked', false);
                $($('#s_funid_D input:radio[name=funid]')[0]).attr('checked', true);

                $('#naoColunas option[value=nome_campus]').remove();
                $('#colunas').append( $("<option/>", {value:'nome_campus', text: '08. Campus'}) );

                atualizaComboCampus($('#entid').val());
            }
        }

        function gerarRelatorio(tipo){
            var formulario = document.formulario;
            var tipo_relatorio = tipo;

            var super_prof  = $('input:radio[name=super_prof]:checked');
            var unid_camp   = $('input:radio[name=unid_camp]:checked');
            var funid       = $('input:radio[name=funid]:checked');
            var entid       = $('#entid');

            var erro;

            if(!super_prof.val()){
                alert('O campo "Superior/ Profissional" é um campo obrigatório!');
                super_prof.focus();
                erro = 1;
                return false;
            }

            if(!entid.val()){
                alert('O campo "Entidade" é um campo obrigatório!');
                entid.focus();
                erro = 1;
                return false;
            }

            if(!unid_camp.val()){
                alert('O campo "Unidade/Campus" é um campo obrigatório!');
                unid_camp.focus();
                erro = 1;
                return false;
            }

            if(!funid.val()){
                alert('O campo "Dirigentes" é um campo obrigatório!');
                funid.focus();
                erro = 1;
                return false;
            }

            selectAllOptions( formulario.colunas );
            selectAllOptions( formulario.camp_id );
            
            if(tipo_relatorio == 'visual'){
                document.getElementById('tipo_relatorio').value = 'visual';
            }else
                if(tipo_relatorio == 'xls'){
                document.getElementById('tipo_relatorio').value = 'xls';
            }else
                if(tipo_relatorio == 'pdf'){
                document.getElementById('tipo_relatorio').value = 'pdf';
            }

            if(!erro){
                var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
                formulario.target = 'relatorio';
                formulario.submit();
            }

            janela.focus();
        }

        function habilitaTipoMembro( opc ){
            var opcao;
            var unid_camp = $('input:radio[name=unid_camp]:checked').val();

            if( opc == 'M' ){
                $('#tpmid').attr('disabled', false);
                $('#tpmid').addClass('obrigatorio');
                $('#td_combo_tpmid').find('img').css('display', '');
            }else{
                $('#tpmid').attr('disabled', true);
                $('#tpmid').removeClass('obrigatorio');
                $('#td_combo_tpmid').find('img').css('display', 'none');
            }

            if( unid_camp != '' && opc == 'M' ){
                opcao = unid_camp;
            }else{
                opcao = 'XXX';
            }

            $.ajax({
                type: "POST",
                url: window.location.href,
                data: "requisicao=atualizaComboTipoMembro&opc=" + opcao,
                async: false,
                success: function (resp) {
                    $('#td_combo_tpmid').html(resp);
                }
            });
        }

        $1_11(document).ready(function (){
            $1_11(window).unload(
                $1_11('.chosen-select').chosen()
            );

            atualizaComboInstituicao('S');
        });
    </script>
</head>

<body>
    <form name="formulario" id="formulario" action="" method="POST">
        <input type="hidden" name="requisicao" id="requisicao" value=""/>
        <input type="hidden" name="tipo_relatorio" id="tipo_relatorio" value=""/>

        <table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
            <tr>
                <td class="subtituloesquerda" colspan="2">
                    DEFINIÇÃO DO RELATÓRIO
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Superior/ Profissional</td>
                <td>
                    <input type="radio" checked="checked" name="super_prof" id="super_prof_s" value="S" style="margin-left:0px;" class="obrigatorio" onclick="atualizaComboInstituicao('S');"> Educação Superior
                    <input type="radio" name="super_prof" id="super_prof_p" value="P" style="margin-left:5px;" class="obrigatorio" onclick="atualizaComboInstituicao('P');"> Educação Profissional
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%"> Entidade </td>
                <td id="td_combo_instituicao">
                    <?PHP
                        $sql = "
                             SELECT e.entid AS codigo,
                                    upper(e.entnome) AS descricao
                               FROM entidade.entidade e2
                               JOIN entidade.entidade e ON e2.entid = e.entid
                               JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
                          LEFT JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
                               JOIN entidade.endereco ed ON ed.entid = e.entid
                               JOIN territorios.municipio mun ON mun.muncod = ed.muncod
                              WHERE 1<>1
                           ORDER BY e.entnome
                        ";
                        $db->monta_combo('entid', $sql, 'S', "Digite o nome e selecione a Unidade...", 'atualizaUnidadeCampus', '', '', 600, 'S', 'entid', '', $entid, 'Unidade', '', 'chosen-select');
                    ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Unidade/Campus</td>
                <td>
                    <input type="radio" name="unid_camp" id="unid_camp_U" value="U" style="margin-left:0px;" class="obrigatorio" onclick="atualizaOpcDirigentes('U');" checked> Unidade
                    <input type="radio" disabled="disabled" name="unid_camp" id="unid_camp_C" value="C" style="margin-left:6px;" class="obrigatorio" onclick="atualizaOpcDirigentes('C');"> Campus
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr id="popup_camp" style="display: none;">
				<td class="SubTituloDireita" width="25%"> Campus </td>
				<td id="td_combo_campus"></td>
			</tr>
            <tr>
                <td class="SubTituloDireita" width="25%"> Dirigentes </td>
                <td>
                    <span id="s_funid_R">
                        <input type="radio" checked="checked" name="funid" id="funid_R" value="21" class="obrigatorio" onclick="habilitaTipoMembro('D');"> 
                        Reitores/Interlocutorres Institucional
                        <input type="radio" name="funid" id="funid_M" value="123" style="margin-left:5px;" class="obrigatorio" onclick="habilitaTipoMembro('M');"> 
                    	Membros dos Conselhos Superior
                    </span>

                    <span id="s_funid_D" style="display:none;">
                        <input type="radio" name="funid" id="funid_D" value="24" class="obrigatorio" onclick="habilitaTipoMembro('D');"> 
                        Dirigentes dos Campus/Interlocutorres Institucional
                        <input type="radio" name="funid" id="funid_M" value="123" style="margin-left:5px;" class="obrigatorio" onclick="habilitaTipoMembro('M');"> 
                    	Membros dos Conselhos de Campus
                    </span>
                    
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Situação</td>
                <td>
                    <input type="radio" name="mcsstatus" id="mcsstatus_T" value="" style="margin-left:0px;" checked> Todos
                    <input type="radio" name="mcsstatus" id="mcsstatus_A" value="t" style="margin-left:5px;"> Ativo
                    <input type="radio" name="mcsstatus" id="mcsstatus_I" value="f" style="margin-left:5px;"> Inatvo
                    <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
                </td>
            </tr>
            <tr>
                <td class="subtituloesquerda" colspan="2">
                    FILTROS
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Colunas</td>
                <td>
                    <?PHP
                        #Montar e exibe colunas
                        $arrayColunasOrig = colunasOrigem();
                        $arrayColunasDest = colunasDestino();
                        $colunas = new Agrupador('formulario');
                        $colunas->setOrigem('naoColunas', null, $arrayColunasOrig);
                        $colunas->setDestino('colunas', null, $arrayColunasDest);
                        $colunas->exibir();
                    ?>
                </td>
            </tr>
        </table>
        <table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    <input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('visual');"/>
                    <input type="button" name="Gerar Relatório" value="Visualizar XLS" onclick="javascript:gerarRelatorio('xls');"/>
                    <input type="button" name="Gerar Relatório" value="Visualizar em PDF" onclick="javascript:gerarRelatorio('pdf');"/>
                </td>
            </tr>
        </table>
    </form>
</body>
</html>

<?PHP
    function colunasOrigem(){
        return array(
            array(
                'codigo'    => 'funid',
                'descricao' => '01. Cargo'
            ),
            array(
                'codigo'    => 'entnome',
                'descricao' => '02. Dirigente'
            ),
            array(
                'codigo'    => 'tpmdsc',
                'descricao' => '03. Tipo de Membro'
            ),
            array(
                'codigo'    => 'mcsvigenciainicial',
                'descricao' => '04. Início da Vigência'
            ),
            array(
                'codigo'    => 'mcsvigenciafinal',
                'descricao' => '05. Fim da Vigência'
            ),
            array(
                'codigo'    => 'mcssituacao',
                'descricao' => '06. Situação'
            ),
            array(
                'codigo'    => 'arqid',
                'descricao' => '07. Ato Legal'
            ),
            array(
                'codigo'    => 'nome_campus',
                'descricao' => '08. Campus'
            ),
        	array(
                'codigo'    => 'endcep',
                'descricao' => '09. CEP'
            ),	
        	array(
        		'codigo'    => 'endlog',
        		'descricao' => '10. Logradouro'
        	),
        	array(
        		'codigo'    => 'endcom',
        		'descricao' => '11. Complemento'
        	),
        	array(
        		'codigo'    => 'endbai',
        		'descricao' => '12. Bairro'
        	),        		
       		array(
  				'codigo'    => 'estuf',
   				'descricao' => '13. UF'
       		),
       		array(
   				'codigo'    => 'mundsc',
   				'descricao' => '14. Município'
       		)
        );
    }

    function colunasDestino(){
        return array(
            array(
                'codigo'    => 'funid',
                'descricao' => '01. Cargo'
            ),
            array(
                'codigo'    => 'entnome',
                'descricao' => '02. Dirigente'
            ),
            array(
                'codigo'    => 'mcsvigenciainicial',
                'descricao' => '04. Início da Vigência'
            ),
            array(
                'codigo'    => 'mcsvigenciafinal',
                'descricao' => '05. Fim da Vigência'
            ),
            array(
                'codigo'    => 'mcssituacao',
                'descricao' => '06. Situação'
            )
        );
    }

?>