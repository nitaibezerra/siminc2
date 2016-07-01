<?PHP
    if($_REQUEST['requisicao']){
        $_REQUEST['requisicao']($_REQUEST);
    }

    if($_SESSION['maismedicomec']['mntid'] == ''){
        $db->sucesso("principal/mantenedora/cad_mantenedora", '', "Ocorreu um problema, é necessário o preenchimento dos dados da Mantida. Tente novamente!");
    }
    
    verificaMantenedora();

    include APPRAIZ . "includes/cabecalho.inc";
    monta_titulo( 'Mais Médicos - MEC', 'Resumo' );

    $abacod_tela    = ABA_CADASTRO_MANTENEDORA;
    $url            = 'maismedicomec.php?modulo=principal/mantenedora/resumo&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);


    if($mnmid == ''){
        $mnmid = $_REQUEST['mnmid'];
       
    }
    if(!empty($_REQUEST['mnmid'])){
        $_SESSION['maismedicomec']['mnmid'] = $_REQUEST['mnmid'];
    }
    else
    {
         unset($_SESSION['maismedicomec']['mnmid']);
    }
    

    if( $mnmid != '' ){
        $dados = buscarDadosMantida( $mnmid );
        
        if( $dados['mnmid'] != '' ){ 
            $habilita_mnmid = 'N';
        }else{
            $habilita_mnmid = 'S';
        }
        $habilita_mnmid = 'N';

        if( $dados['mtdimovelsit'] == 'P' ){
            $checked_p = 'checked="checked"';
        }elseif( $dados['mtdimovelsit'] == 'A' ){
            $checked_a = 'checked="checked"';
        }elseif( $dados['mtdimovelsit'] == 'O' ){
            $checked_o = 'checked="checked"';
        }else{
            $checked_p = '';
            $checked_a = '';
            $checked_o = '';
        }

        #FACULDADE
        if( $dados['mtdfaculdade'] == 't' ){
            $ck_s_mtdfaculdade = 'checked="checked"';
        }elseif( $dados['mtdfaculdade'] == 'f' ){
            $ck_n_mtdfaculdade = 'checked="checked"';
        }else{
            $ck_s_mtdfaculdade = '';
            $ck_n_mtdfaculdade = '';
        }
        #CENTRO UNIVERSITARIO
        if( $dados['mtdcentrouniversitario'] == 't' ){
            $ck_s_mtdcentrouniversitario = 'checked="checked"';
        }elseif( $dados['mtdcentrouniversitario'] == 'f' ){
            $ck_n_mtdcentrouniversitario = 'checked="checked"';
        }else{
            $ck_s_mtdcentrouniversitario = '';
            $ck_n_mtdcentrouniversitario = '';
        }
        #UNIVERSIDADE
        if( $dados['mtduniversidade'] == 't' ){
            $ck_s_mtduniversidade = 'checked="checked"';
        }elseif( $dados['mtduniversidade'] == 'f' ){
            $ck_n_mtduniversidade = 'checked="checked"';
        }else{
            $ck_s_mtduniversidade = '';
            $ck_n_mtduniversidade = '';
        }

        #FILANTROPIA
        if( $dados['mtdfilantropica'] == "t"){
            $ck_s_mtdfilantropica = 'checked="checked"';
        }elseif( $dados['mtdfilantropica'] == "f" ){
            $ck_n_mtdfilantropica = 'checked="checked"';
        }else{
            $ck_s_mtdfilantropica = '';
            $ck_n_mtdfilantropica = '';
        }

        #COMUNITÁRIA
        if( $dados['mtdcomunitaria'] == "t"){
            $ck_s_mtdcomunitaria = 'checked="checked"';
        }elseif( $dados['mtdcomunitaria'] == "f" ){
            $ck_n_mtdcomunitaria = 'checked="checked"';
        }else{
            $ck_s_mtdcomunitaria = '';
            $ck_n_mtdcomunitaria = '';
        }

        #CONFESSIONAL
        if( $dados['mtdconfessional'] == "t"){
            $ck_s_mtdconfessional = 'checked="checked"';
        }elseif( $dados['mtdconfessional'] == "f" ){
            $ck_n_mtdconfessional = 'checked="checked"';
        }else{
            $ck_s_mtdconfessional = '';
            $ck_n_mtdconfessional = '';
        }

        #AUTORIZAÇÃO
        //$ck_s_mtdautfuncionamento $ck_s_mtdcredenciamento
        if( $dados['mtdautfuncionamento'] == "t"){
            $ck_s_mtdautfuncionamento = 'checked="checked"';
        }elseif( $dados['mtdautfuncionamento'] == "f" ){
            $ck_n_mtdautfuncionamento = 'checked="checked"';
        }else{
            $ck_s_mtdautfuncionamento = '';
            $ck_n_mtdautfuncionamento = '';
        }
        #CREDENCIAMENTO
        if( $dados['mtdcredenciamento'] == "t"){
            $ck_s_mtdcredenciamento = 'checked="checked"';
        }elseif( $dados['mtdcredenciamento'] == "f" ){
            $ck_n_mtdcredenciamento = 'checked="checked"';
        }else{
            $ck_s_mtdcredenciamento = '';
            $ck_n_mtdcredenciamento = '';
        }
        
        #PRIVADA
        if( $dados['mtdprivadacomlucro'] == "t"){
            $ck_s_mtdprivadacomlucro = 'checked="checked"';
        }elseif( $dados['mtdprivadacomlucro'] == "f" ){
            $ck_n_mtdprivadacomlucro = 'checked="checked"';
        }else{
            $ck_s_mtdprivadacomlucro = '';
            $ck_n_mtdprivadacomlucro = '';
        }
        
    	#PRIVADA SEM LUCRATIVOS
        if( $dados['mtdprivadasemlucro'] == "t"){
            $ck_s_mtdprivadasemlucro = 'checked="checked"';
        }elseif( $dados['mtdprivadasemlucro'] == "f" ){
            $ck_n_mtdprivadasemlucro = 'checked="checked"';
        }else{
            $ck_s_mtdprivadasemlucro = '';
            $ck_n_mtdprivadasemlucro = '';
        }
    }
     $_SESSION['maismedicomec']['mnmid'] = $mnmid;
    $perfil = pegaPerfilGeral();
?>

<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/jquery-autocomplete/jquery.autocomplete.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script	type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>

<script type="text/javascript">

	
    $1_11(document).ready(function(){
        //#VERSÃO JQUERY INSTANCIADA NO CABEÇALHO
        $1_11(window).unload(
            $1_11('.chosen-select').chosen()
        );
    });
    
    function abrirPopupSelecaoMantida(){
        var url = 'maismedicomec.php?modulo=principal/mantenedora/popup_selecione_mantida&acao=A';        
        window.open(url, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=800,height=650');
    }

    function atualizaComboMunicipioMantenedora( estuf ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboMunicipioMantenedora&estuf="+estuf,
            async: false,
            success: function(resp){
                $('#td_combo_municipio').html(resp);
            }
        });
    }

    function buscarDadosMantidaGD( codigo ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarDadosMantidaGD&codigo="+codigo,
            success: function(resp){
                var dados = $.parseJSON(resp);

                $('#mtdid').val(dados.iesid);
                $('#mtddsc').val(dados.iesdsc);
                $('#mntsigla').val(dados.iessigla);
                $('#mtdcep').val(dados.iescep);
                $('#mtdsite').val(dados.iessite);
                
                var cep = trim(dados.iescep);
                buscarEndereceCEP( cep.replace("-", "") );
            }
        });
    }

    function buscarEndereceCEP( cep ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=buscarEndereceCEP&cep="+cep,
            success: function(resp){
                var dados = $.parseJSON(resp);

                $('#mtdlogradouro').val(dados.logradouro);
                $('#mtdbairro').val(dados.bairro);
                $('#estuf').val(dados.estado);
                $('#muncod').val(dados.muncod);
            }
        });
    }

    function buscarDadosEditarMantida( mnmid ){
        
        window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/resumo&acao=A&mnmid='+mnmid;
    }

    function excluirDadosMantida( mnmid ){
        var confirma = confirm("Deseja realmente excluir o Registro?");
        if( confirma ){
            $.ajax({
                type    : "POST",
                url     : window.location,
                data    : "requisicao=excluirDadosMantida&mnmid="+mnmid,
                success: function(resp){
                    if( trim( resp ) == 'OK' ){
                        alert('Operação Realizada com sucesso!');
                        window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/resumo&acao=A';
                    }
                }
            });
        }
    }

    function resetFromulario(){
    	
        $("#mtdid").attr('readonly', false);
        $("#mtdid").removeClass('disabled');
        $("#mtdid").addClass('obrigatorio normal');
        
        $.each($("input[type=text], input[type=hidden], select, textarea, input[type=radio], input[type=checkbox]"), function(i,v){
            if( $(this).attr('type') != 'radio' && $(this).attr('type') != 'checkbox'){
                $(v).val('');
            }
            if( $(this).attr('type') == 'radio' ){
                $(v).attr('checked', false);
            }
            if( $(this).attr('type') == 'checkbox' ){
                $(v).attr('checked', false);
            }
        });
        
    }
    
    function resetFromulario2(){
    	location.href="maismedicomec.php?modulo=principal/mantenedora/resumo&acao=A";
    }

    function salvarDadosMatida(){
        var erro;
        var radio_entro;
        var check_entro;
        var campos = '';

        $.each($(".obrigatorio"), function(i, v){
            if( ($(this).attr('type') != 'radio') && ($(this).attr('type') != 'checkbox') ){
                if ( $(v).val() == '' ){
                    erro = 1;
                    campos += '- ' + $(this).attr('title') + " \n";
                }
            }else
                if( $(this).attr('type') == 'radio' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var radio_box = $('input:radio[name='+name+']:checked');

                    if(!radio_box.val() && (name != radio_entro) ){
                        erro = 1;
                        radio_entro = name;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
            }else
                if( $(this).attr('type') == 'checkbox' ){
                    var name  = $(this).attr('name');
                    var value = $(this).attr('value');
                    var check_box = $('input:checkbox[name='+name+']:checked');

                    if(!check_box.val() && (name != check_entro) ){
                        erro = 1;
                        check_entro = name;
                        campos += '- ' + $(this).attr('title') + " \n";
                    }
            }
        });

        if( erro > 0 ){
            alert('Existem "campos obrigatorios" vazios! São eles: \n'+campos);
            return false;
        }
        
        if(document.getElementById("mtdfaculdade_s").checked == true || document.getElementById("mtdcentrouniversitario_s").checked == true){
        	if(document.getElementById("mantida_muncod").value != document.getElementById("muncod").value){
        		alert('O Município para qual concorre deve ser igual o Município do endereço.');
            	return false;
        	}
        }
        
        if(document.getElementById("mtduniversidade_s").checked == true){
        	var e = document.getElementById("mantida_muncod");
			var itemSelecionado = e.options[e.selectedIndex].text;
			var uf = itemSelecionado.split(' - ');
			
			if(uf[1] != document.getElementById("estuf").value){
        		alert('A UF para qual concorre deve ser igual a UF do endereço.');
            	return false;
        	}
        }
        
        if(!erro){
            $('#requisicao').val('salvarDadosMatida');
            $('#formulario').submit();
        }
    }

    function voltarPaginaPrincipal(){
        var janela = window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/lista_grid_mantenedora&acao=A';
        janela.focus();
    }

    function imprimirRecibo(){
        var url = 'maismedicomec.php?modulo=principal/mantenedora/popup_mantida&acao=A';        
        window.open(url, '', 'toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=300,height=250');
        
    }
    
    
    function pesquisarMantida(){
        
        window.location.href = 'maismedicomec.php?modulo=principal/mantenedora/resumo&acao=A&mun='+document.formulario.mantida_muncod.value+'&nome='+document.formulario.mtddsc.value+'&sigla='+document.formulario.mntsigla.value;
    }
    
</script>

<?php $mntid = $_SESSION['maismedicomec']['mntid'];
$sql = "select * from maismedicomec.mantenedora where mntid = ".$mntid;
$dados_mantenedora = $db->carregar($sql);


  $sql1 = "
        SELECT  count(m.mtdid) as quantidade
        FROM maismedicomec.mantida AS m

        JOIN maismedicomec.mantidamunicipio AS mm ON mm.mtdid = m.mtdid
        JOIN territorios.municipio AS mun ON mm.muncod = mun.muncod
      
        JOIN seguranca.perfilusuario pu on m.usucpf = pu.usucpf
         JOIN seguranca.perfil pe on pe.pflcod = pu.pflcod and pe.sisid = {$_SESSION['sisid']}

        WHERE mtdstatus = 'A' AND mm.mntid = {$_SESSION['maismedicomec']['mntid']}";
    
        if(in_array(PERFIL_HOSPITAL, $perfil)){            
            $sql1 .= " and pe.pflcod = ".PERFIL_HOSPITAL;
            $sql1 .= " and m.usucpf = '{$_SESSION['usucpf']}'";
        }
        
        if(in_array(PERFIL_IES, $perfil)){            
            $sql1 .= " and pe.pflcod = ".PERFIL_IES;
            $sql1 .= " and m.usucpf = '{$_SESSION['usucpf']}'";
        }
        
    
    
   $RS = $db->recuperar($sql1,$res);
   $RS['quantidade'];

?>
   <table align="center" border="0" class="tabela Listagem" cellpadding="3" cellspacing="1">

        <tr>
            <td class ="SubTituloDireita" width="35%">Código mantenedora: </td>
            <td><?php echo $mntid ?></td>
        </tr>
        
          <tr>
            <td class ="SubTituloDireita" width="35%">Razão social</td>
            <td><?php echo $dados_mantenedora[0][mntrazaosocial];?></td>
        </tr>
   </table>


<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="mm_mtdid" name="mm_mtdid" value="<?=$dados['mm_mtdid'];?>"/>
    <input type="hidden" id="mnmid" name="mnmid" value="<?=$dados['mnmid']?>"/>

    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
    	    <tr>
                <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Mantida(s) Indicada(s) </td>
            </tr>
            <tr>
                <td colspan="3" >
                    
                    <?
						$sql = "
						        SELECT  mm.mnmid || '/2015' as inscricao,
						        		mnmprioridade,
						                mtddsc,
						                mntsigla,
						                mundescricao || ' - ' || mun.estuf as mun,
						                mtdtelefone01,
						                mtdsite --, pe.pfldsc
						        FROM maismedicomec.mantida AS m
						        JOIN maismedicomec.mantidamunicipio AS mm ON mm.mtdid = m.mtdid
						        left JOIN territorios.municipio AS mun ON mm.muncod = mun.muncod
						        JOIN seguranca.perfilusuario pu on m.usucpf = pu.usucpf
						        JOIN seguranca.perfil pe on pe.pflcod = pu.pflcod and pe.sisid = {$_SESSION['sisid']}
						        WHERE mtdstatus = 'A' AND mm.mntid = {$_SESSION['maismedicomec']['mntid']}
						        ORDER BY 2
						    	";
						
						    $cabecalho = array("Nº de Inscrição","Prioridade","Nome", "Sigla", "Município", 'Telefone 01', 'Site');
						    $alinhamento = Array('center','center','','','','','','','','','');
						    $tamanho = Array('','','','','','','','','','','');
						    $db->monta_lista($sql, $cabecalho, 1000, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
                    
                    ?>
                    
                </td>
            </tr>
            <tr>
                <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Nova(s) Mantida(s) </td>
            </tr>
            <tr>
                <td colspan="3" >
                    
                    <?
						$sql = "
						        SELECT  mm.mnmid || '/2015' as inscricao,
						        		mnmprioridade,
						                mtddsc,
						                mntsigla,
						                mundescricao || ' - ' || mun.estuf as mun,
						                mtdtelefone01,
						                mtdsite --, pe.pfldsc
						        FROM maismedicomec.mantidacandidata AS m
						        JOIN maismedicomec.mantidamunicipiocandidata AS mm ON mm.mtdid = m.mtdid
						       	left JOIN territorios.municipio AS mun ON mm.muncod = mun.muncod
						        JOIN seguranca.perfilusuario pu on m.usucpf = pu.usucpf
						        JOIN seguranca.perfil pe on pe.pflcod = pu.pflcod and pe.sisid = {$_SESSION['sisid']}
						        WHERE mtdstatus = 'A' AND mm.mntid = {$_SESSION['maismedicomec']['mntid']}
						        ORDER BY 2
						    	";
						
						    $cabecalho = array("Nº de Inscrição","Prioridade","Nome", "Sigla", "Município", 'Telefone 01', 'Site');
						    $alinhamento = Array('center','center','','','','','','','','','');
						    $tamanho = Array('','','','','','','','','','','');
						    $db->monta_lista($sql, $cabecalho, 1000, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
                    
                    ?>
                    
                </td>
            </tr>
            <tr>
                <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Corpo Dirigente </td>
            </tr>
            <tr>
                <td colspan="3" >
                    
                    <?
						 $sql = "
						        SELECT  DISTINCT 
						                trim( replace( to_char( cast(c.cpdcpf as bigint), '000:000:000-00' ), ':', '.' ) ) AS cpdcpf,
						                c.cpdnome,
						                c.cpdtelefonecomercial,
						                c.cpdtelcelular,
						                c.cpdemail,
						                c.cpdcargo,
						                CASE WHEN dmd.dmdid IS NOT NULL and md.mtddsc IS NOT NULL and (select count(mnmid) from maismedicomec.mantidamunicipio where mntid = {$_SESSION['maismedicomec']['mntid']}) > 0 THEN md.mtddsc
								     		 WHEN dmd.dmdid IS NOT NULL and md2.mtddsc IS NOT NULL and (select count(mnmid) from maismedicomec.mantidamunicipiocandidata where mntid = {$_SESSION['maismedicomec']['mntid']}) > 0 THEN md2.mtddsc
						                     ELSE '-'
						                END AS mtddsc
						        FROM maismedicomec.corpodirigente AS c
						        LEFT JOIN maismedicomec.dirigentemantenedora AS dmt ON dmt.cpdid = c.cpdid
						        LEFT JOIN maismedicomec.dirigentemantida AS dmd ON dmd.cpdid = c.cpdid
						        LEFT JOIN maismedicomec.mantenedora AS mt ON mt.mntid = dmt.mntid
						        LEFT JOIN maismedicomec.mantida AS md ON md.mtdid = dmd.mtdid AND md.mtdstatus = 'A'
						        LEFT JOIN maismedicomec.mantidamunicipio AS mm ON mm.mntid = mt.mntid OR mm.mtdid = md.mtdid
						        LEFT JOIN maismedicomec.mantidacandidata AS md2 ON md2.mtdid = dmd.mtdid AND md2.mtdstatus = 'A'
						        LEFT JOIN maismedicomec.mantidamunicipiocandidata AS mm2 ON mm2.mntid = mt.mntid OR mm2.mtdid = md2.mtdid
						        WHERE c.cpdstatus='A' and (mm.mntid = {$_SESSION['maismedicomec']['mntid']} OR mm2.mntid = {$_SESSION['maismedicomec']['mntid']})
						        ORDER BY 2
						    ";
						    
						    $cabecalho = array("CPF", "Nome", "Tel.Comercial", "Tel. Celular", "E-mail", "Cargo", "Mantida");
						    $alinhamento = Array('center', '', '', '', '','','','','');
						    $tamanho = Array('', '', '', '', '','','','');
						    $db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
                    
                    ?>
                    
                </td>
            </tr>
        	<tr>
                <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Portfólio Mantenedora </td>
            </tr>
            <tr>
                <td colspan="3" >
                	
                    <?
						$sql = "SELECT merid, mermantidaindicada1, mermantidaindicada2, mermantidaindicada3, 
								       merinexistsupervinstitucional, 
								       usucpf, merdtinclusao, merstatus
					  			FROM maismedicomec.mantenedoraexpregulatoria
					  			WHERE merstatus = 'A' and mntid = ".$_SESSION['maismedicomec']['mntid'];
					    $dados = $db->carregar($sql);
					    if($dados[0]) extract($dados[0]);
					    
					    //recupera muncod mantenedora
					    $sql = "SELECT muncod
					  			FROM maismedicomec.mantenedora
					  			WHERE mntid = ".$_SESSION['maismedicomec']['mntid'];
					    $muncodMantenedora = $db->pegaUm($sql);
                	?>
                    
                    <table align="center" bgcolor="#f5f5f5" border="1" cellpadding="3" cellspacing="1" class="tabela listagem">
				        <tr>
				            <td width="35%" class ="SubTituloDireita">Mantida 1: </td>
				            <td colspan="2">
				                <?PHP
									if(!$mermantidaindicada1){
										$mermantidaindicada1 = '0';
									}
									
									/*
									$sql = "
				                        SELECT  m.mtdid AS codigo,
				                                m.mtddsc AS descricao
				                        FROM maismedicomec.mantida AS m
				                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.mtdid = m.mtdid
				                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
				                        ORDER BY descricao
				                    ";
									 */
				                    $sql = "
				                        SELECT  m.iesid AS codigo,
				                                m.iesdsc AS descricao
				                        FROM gestaodocumentos.instituicaoensino AS m
				                        WHERE m.mntid = {$_SESSION['maismedicomec']['mntid']}
				                        
				                        and m.iesid not in (select mtdid 
				                        					from maismedicomec.mantidamunicipio
				                        					where mntid = {$_SESSION['maismedicomec']['mntid']}
				                        					)
				                        					
				                        and m.iesid = '{$mermantidaindicada1}'
				                        ORDER BY 2
				                    ";
				                    $dadosMantida = $db->pegaLinha($sql);
				                    
				                    if($dadosMantida){
				                    	echo $dadosMantida['descricao'];
				                    }else{
				                    	echo 'Nenhuma Mantida adicionada.';
				                    }
				                    
				                    //$db->monta_combo('mermantidaindicada1', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mermantidaindicada1', '', $mermantidaindicada1, 'Mantida Indicada 1', 'onchange="vMantida(1);"');
				                    
				                    
				                ?>
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Curso de medicina: </td>
				            <td colspan="2">
				                <?
				                if($mermantidaindicada1){
				                	$sql = "select crmid from maismedicomec.mantenedoraareamedica 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada1}
				                			order by mamid";
				                	$dadosMedicina1 = $db->carregar($sql);
				                	if(!$dadosMedicina1){
				                		$totalMedicina1 = 0;
				                	}else{
				                		$totalMedicina1 = count($dadosMedicina1);
				                	}
				                }else{
				                	$totalMedicina1 = 0;
				                	$_POST['ctcursomedicina1'] = 0;
				                }
				                
				                
								$ctcursomedicina1 = $_POST['ctcursomedicina1'] ? $_POST['ctcursomedicina1'] : $totalMedicina1;
								
								if($ctcursomedicina1==0) echo 'Nenhum curso adicionado.';
								
								for($i=1; $i<=$ctcursomedicina1; $i++){
									/*
									$sql = "
					                        SELECT  m.crmid AS codigo,
					                                m.crmcodcurso||' - '||m.crmdsccurso AS descricao
					                        FROM maismedicomec.cursomedicina AS m
					                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
					                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
					                        ORDER BY descricao";
									*/
									if($mermantidaindicada1){
										$sql = "SELECT  min(m.crmid) AS codigo,
						                                m.crmdsccurso AS descricao
						                        FROM maismedicomec.cursomedicina AS m
						                        WHERE m.iesid = {$mermantidaindicada1}
						                        group by m.crmdsccurso
						                        ORDER BY 2";
									}
									
									$mercursomedicina1 = "";
									if($_POST['submite']){
										$mercursomedicina1 = $_POST['mercursomedicina1'][($i-1)];
									}else{
										$mercursomedicina1 = $dadosMedicina1[($i-1)];
									}

									echo $db->pegaUm("SELECT crmdsccurso from maismedicomec.cursomedicina where crmid = {$mercursomedicina1['crmid']}");
									
					                //$db->monta_combo('mercursomedicina1[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursomedicina1', '', $mercursomedicina1, 'Curso de medicina');
					                
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursomedicina1\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctcursomedicina1" id="ctcursomedicina1" value="<?=$ctcursomedicina1?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Curso área de saúde: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada1){
				                	$sql = "select casid from maismedicomec.mantenedoraareasaude 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada1}
				                			order by masid";
				                	$dadosSaude1 = $db->carregar($sql);
				                	if(!$dadosSaude1){
				                		$totalSaude1 = 0;
				                	}else{
				                		$totalSaude1 = count($dadosSaude1);
				                	}
				                }else{
				                	$totalSaude1 = 0;
				                	$_POST['ctcursosaude1'] = 0;
				                }
				                
								$ctcursosaude1 = $_POST['ctcursosaude1'] ? $_POST['ctcursosaude1'] : $totalSaude1;
								
								if($ctcursosaude1==0) echo 'Nenhum curso adicionado.';
								
								for($i=1; $i<=$ctcursosaude1; $i++){
									/*
									$sql = "
					                        SELECT  m.casid AS codigo,
					                                m.cascodcurso||' - '||m.casdsccurso AS descricao
					                        FROM maismedicomec.cursoareasaude AS m
					                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
					                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
					                        ORDER BY descricao
					                    ";
									*/
									
									if($mermantidaindicada1){
										$sql = "SELECT  min(casid) AS codigo, m.casdsccurso AS descricao
						                        FROM maismedicomec.cursoareasaude AS m
						                        WHERE m.iesid = {$mermantidaindicada1} 
						                        group by m.casdsccurso
						                        ORDER BY 2";
									}
									
									
									$mercursosaude1 = "";
									if($_POST['submite']){
										$mercursosaude1 = $_POST['mercursosaude1'][($i-1)];
									}else{
										$mercursosaude1 = $dadosSaude1[($i-1)];
									}
									
									
									echo $db->pegaUm("SELECT casdsccurso from maismedicomec.cursoareasaude where casid = {$mercursosaude1['casid']}");
									
					                //$db->monta_combo('mercursosaude1[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursosaude1', '', $mercursosaude1, 'Curso de saúde');
					                
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursosaude1\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctcursosaude1" id="ctcursosaude1" value="<?=$ctcursosaude1?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Programa residência: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada1){
				                	$sql = "select mprdsc from maismedicomec.mantenedoraprogresidencia 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada1}
				                			order by mprid";
				                	$dadosResidencia1 = $db->carregarColuna($sql);
				                	if(!$dadosResidencia1){
				                		$totalResidencia1 = 0;
				                	}else{
				                		$totalResidencia1 = count($dadosResidencia1);
				                	}
				                }else{
				                	$totalResidencia1 = 0;
				                	$_POST['ctresidencia1'] = 0;
				                }
				                
								$ctresidencia1 = $_POST['ctresidencia1'] ? $_POST['ctresidencia1'] : $totalResidencia1;
								
								if($ctresidencia1==0) echo 'Nenhum programa adicionado.';
								
								for($i=1; $i<=$ctresidencia1; $i++){
									
									$merresidencia1 = "";
									if($_POST['submite']){
										$merresidencia1 = $_POST['merresidencia1'][($i-1)];
									}else{
										$merresidencia1 = $dadosResidencia1[($i-1)];
									}
									
									echo $merresidencia1;
									
					                //echo campo_texto('merresidencia1[]', 'N', 'S', 'Programa Residência', 59, 100, '', '', '', '', 0, 'id="merresidencia1"', '', $merresidencia1, '', null);
				
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctresidencia1\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctresidencia1" id="ctresidencia1" value="<?=$ctresidencia1?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Mestrado/doutorado: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada1){
				                	$sql = "select mpmdsc from maismedicomec.mantenedoraprogmestdoutorado 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada1}
				                			order by mpmid";
				                	$dadosMestrado1 = $db->carregarColuna($sql);
				                	if(!$dadosMestrado1){
				                		$totalMestrado1 = 0;
				                	}else{
				                		$totalMestrado1 = count($dadosMestrado1);
				                	}
				                }else{
				                	$totalMestrado1 = 0;
				                	$_POST['ctmestrado1'] = 0;
				                }
				                
								$ctmestrado1 = $_POST['ctmestrado1'] ? $_POST['ctmestrado1'] : $totalMestrado1;
								
								if($ctmestrado1==0) echo 'Nenhum mestrado/doutorado adicionado.';
								
								for($i=1; $i<=$ctmestrado1; $i++){
									
									$mermestrado1 = "";
									if($_POST['submite']){
										$mermestrado1 = $_POST['mermestrado1'][($i-1)];
									}else{
										$mermestrado1 = $dadosMestrado1[($i-1)];
									}
									
									echo $mermestrado1;
									
					                //echo campo_texto('mermestrado1[]', 'N', 'S', 'Programa mestrado/doutorado', 59, 100, '', '', '', '', 0, 'id="mermestrado1"', '', $mermestrado1, '', null);
					                 
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctmestrado1\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctmestrado1" id="ctmestrado1" value="<?=$ctmestrado1?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita" colspan="2">&nbsp;</td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Mantida 2: </td>
				            <td colspan="2">
				                <?PHP
									if(!$mermantidaindicada2){
										$mermantidaindicada2 = '0';
									}
									
									
				                    $sql = "
				                        SELECT  m.iesid AS codigo,
				                                m.iesdsc AS descricao
				                        FROM gestaodocumentos.instituicaoensino AS m
				                        WHERE m.mntid = {$_SESSION['maismedicomec']['mntid']}
				                        and m.iesid not in (select mtdid 
				                        					from maismedicomec.mantidamunicipio
				                        					where mntid = {$_SESSION['maismedicomec']['mntid']}
				                        					)
				                    	and m.iesid = '{$mermantidaindicada2}'
				                        ORDER BY 2
				                    ";
				                    $dadosMantida = $db->pegaLinha($sql);
				                    
				                    if($dadosMantida){
				                    	echo $dadosMantida['descricao'];
				                    }else{
				                    	echo 'Nenhuma Mantida adicionada.';
				                    }
									
				                    //$db->monta_combo('mermantidaindicada2', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mermantidaindicada2', '', $mermantidaindicada2, 'Mantida Indicada 2', 'onchange="vMantida(2);"');
				                ?>
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Curso de medicina: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada2){
				                	$sql = "select crmid from maismedicomec.mantenedoraareamedica 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada2}
				                			order by mamid";
				                	$dadosMedicina2 = $db->carregar($sql);
				                	if(!$dadosMedicina2){
				                		$totalMedicina2 = 0;
				                	}else{
				                		$totalMedicina2 = count($dadosMedicina2);
				                	}
				                }else{
				                	$totalMedicina2 = 0;
				                	$_POST['ctcursomedicina2'] = 0;
				                }
				                
								$ctcursomedicina2 = $_POST['ctcursomedicina2'] ? $_POST['ctcursomedicina2'] : $totalMedicina2;
								
								if($ctcursomedicina2==0) echo 'Nenhum curso adicionado.';
								
								for($i=1; $i<=$ctcursomedicina2; $i++){
									/*
									$sql = "
					                        SELECT  m.crmid AS codigo,
					                                m.crmcodcurso||' - '||m.crmdsccurso AS descricao
					                        FROM maismedicomec.cursomedicina AS m
					                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
					                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
					                        ORDER BY descricao
					                    ";
									*/
									if($mermantidaindicada2){
										$sql = "SELECT  min(m.crmid) AS codigo,
						                                m.crmdsccurso AS descricao
						                        FROM maismedicomec.cursomedicina AS m
						                        WHERE m.iesid = {$mermantidaindicada2}
						                        group by m.crmdsccurso
						                        ORDER BY 2";
									}
									
									$mercursomedicina2 = "";
									if($_POST['submite']){
										$mercursomedicina2 = $_POST['mercursomedicina2'][($i-1)];
									}else{
										$mercursomedicina2 = $dadosMedicina2[($i-1)];
									}
									
									echo $db->pegaUm("SELECT crmdsccurso from maismedicomec.cursomedicina where crmid = {$mercursomedicina2['crmid']}");
									
					                //$db->monta_combo('mercursomedicina2[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursomedicina2', '', $mercursomedicina2, 'Curso de medicina');
					                
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursomedicina2\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctcursomedicina2" id="ctcursomedicina2" value="<?=$ctcursomedicina2?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Curso área de saúde: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada2){
				                	$sql = "select casid from maismedicomec.mantenedoraareasaude 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada2}
				                			order by masid";
				                	$dadosSaude2 = $db->carregar($sql);
				                	if(!$dadosSaude2){
				                		$totalSaude2 = 0;
				                	}else{
				                		$totalSaude2 = count($dadosSaude2);
				                	}
				                }else{
				                	$totalSaude2 = 0;
				                	$_POST['ctcursosaude2'] = 0;
				                }
				                
								$ctcursosaude2 = $_POST['ctcursosaude2'] ? $_POST['ctcursosaude2'] : $totalSaude2;
								
								if($ctcursosaude2==0) echo 'Nenhum curso adicionado.';
								
								for($i=1; $i<=$ctcursosaude2; $i++){
									/*
									$sql = "
					                        SELECT  m.casid AS codigo,
					                                m.cascodcurso||' - '||m.casdsccurso AS descricao
					                        FROM maismedicomec.cursoareasaude AS m
					                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
					                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
					                        ORDER BY descricao
					                    ";
									*/
									
									if($mermantidaindicada2){
										$sql = "SELECT  min(casid) AS codigo, m.casdsccurso AS descricao
						                        FROM maismedicomec.cursoareasaude AS m
						                        WHERE m.iesid = {$mermantidaindicada2} 
						                        group by m.casdsccurso
						                        ORDER BY 2";
									}
									
									$mercursosaude2 = "";
									if($_POST['submite']){
										$mercursosaude2 = $_POST['mercursosaude2'][($i-1)];
									}else{
										$mercursosaude2 = $dadosSaude2[($i-1)];
									}
									
									echo $db->pegaUm("SELECT casdsccurso from maismedicomec.cursoareasaude where casid = {$mercursosaude2['casid']}");
									
					                //$db->monta_combo('mercursosaude2[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursosaude2', '', $mercursosaude2, 'Curso de saúde');
					                
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursosaude2\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctcursosaude2" id="ctcursosaude2" value="<?=$ctcursosaude2?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Programa residência: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada2){
				                	$sql = "select mprdsc from maismedicomec.mantenedoraprogresidencia 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada2}
				                			order by mprid";
				                	$dadosResidencia2 = $db->carregarColuna($sql);
				                	if(!$dadosResidencia2){
				                		$totalResidencia2 = 0;
				                	}else{
				                		$totalResidencia2 = count($dadosResidencia2);
				                	}
				                }else{
				                	$totalResidencia2 = 0;
				                	$_POST['ctresidencia2'] = 0;
				                }
				                
								$ctresidencia2 = $_POST['ctresidencia2'] ? $_POST['ctresidencia2'] : $totalResidencia2;
								
								if($ctresidencia2==0) echo 'Nenhum programa adicionado.';
								
								for($i=1; $i<=$ctresidencia2; $i++){
									
									
									$merresidencia2 = "";
									if($_POST['submite']){
										$merresidencia2 = $_POST['merresidencia2'][($i-1)];
									}else{
										$merresidencia2 = $dadosResidencia2[($i-1)];
									}
									
									echo $merresidencia2;
									
					                //echo campo_texto('merresidencia2[]', 'N', 'S', 'Programa Residência', 59, 100, '', '', '', '', 0, 'id="merresidencia2"', '', $merresidencia2, '', null);
					                 
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctresidencia2\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctresidencia2" id="ctresidencia2" value="<?=$ctresidencia2?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Mestrado/doutorado: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada2){
				                	$sql = "select mpmdsc from maismedicomec.mantenedoraprogmestdoutorado 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada2}
				                			order by mpmid";
				                	$dadosMestrado2 = $db->carregarColuna($sql);
				                	if(!$dadosMestrado2){
				                		$totalMestrado2 = 0;
				                	}else{
				                		$totalMestrado2 = count($dadosMestrado2);
				                	}
				                }else{
				                	$totalMestrado2 = 0;
				                	$_POST['ctmestrado2'] = 0;
				                }
				                
								$ctmestrado2 = $_POST['ctmestrado2'] ? $_POST['ctmestrado2'] : $totalMestrado2;
								
								if($ctmestrado2==0) echo 'Nenhum mestrado/doutorado adicionado.';
								
								for($i=1; $i<=$ctmestrado2; $i++){
									
									$mermestrado2 = "";
									if($_POST['submite']){
										$mermestrado2 = $_POST['mermestrado2'][($i-1)];
									}else{
										$mermestrado2 = $dadosMestrado2[($i-1)];
									}
									
									echo $mermestrado2;
									
					                //echo campo_texto('mermestrado2[]', 'N', 'S', 'Programa mestrado/doutorado', 59, 100, '', '', '', '', 0, 'id="mermestrado2"', '', $mermestrado2, '', null);
					                
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctmestrado2\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctmestrado2" id="ctmestrado2" value="<?=$ctmestrado2?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita" colspan="2">&nbsp;</td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Mantida 3: </td>
				            <td colspan="2">
				                <?PHP
									if(!$mermantidaindicada3){
										$mermantidaindicada3 = 0;
									}
									
									
				                    $sql = "
				                        SELECT  m.iesid AS codigo,
				                                m.iesdsc AS descricao
				                        FROM gestaodocumentos.instituicaoensino AS m
				                        WHERE m.mntid = {$_SESSION['maismedicomec']['mntid']}
				                        and m.iesid not in (select mtdid 
				                        					from maismedicomec.mantidamunicipio
				                        					where mntid = {$_SESSION['maismedicomec']['mntid']}
				                        					)
				                    	and m.iesid = '{$mermantidaindicada3}'
				                        ORDER BY 2
				                    ";
				                    $dadosMantida = $db->pegaLinha($sql);
				                    
				                    if($dadosMantida){
				                    	echo $dadosMantida['descricao'];
				                    }else{
				                    	echo 'Nenhuma Mantida adicionada.';
				                    }
				                    
				                ?>
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Curso de medicina: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada3){
				                	$sql = "select crmid from maismedicomec.mantenedoraareamedica 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada3}
				                			order by mamid";
				                	$dadosMedicina3 = $db->carregar($sql);
				                	if(!$dadosMedicina3){
				                		$totalMedicina3 = 0;
				                	}else{
				                		$totalMedicina3 = count($dadosMedicina3);
				                	}
				                }else{
				                	$totalMedicina3 = 0;
				                	$_POST['ctcursomedicina3'] = 0;
				                }
				                
								$ctcursomedicina3 = $_POST['ctcursomedicina3'] ? $_POST['ctcursomedicina3'] : $totalMedicina3;
								
								if($ctcursomedicina3==0) echo 'Nenhum curso adicionado.';
								
								for($i=1; $i<=$ctcursomedicina3; $i++){
									/*
									$sql = "
					                        SELECT  m.crmid AS codigo,
					                                m.crmcodcurso||' - '||m.crmdsccurso AS descricao
					                        FROM maismedicomec.cursomedicina AS m
					                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
					                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
					                        ORDER BY descricao
					                    ";
									*/
									
									if($mermantidaindicada3){
										$sql = "SELECT  min(m.crmid) AS codigo,
						                                m.crmdsccurso AS descricao
						                        FROM maismedicomec.cursomedicina AS m
						                        WHERE m.iesid = {$mermantidaindicada3}
						                        group by m.crmdsccurso
						                        ORDER BY 2";
									}
									
									$mercursomedicina3 = "";
									if($_POST['submite']){
										$mercursomedicina3 = $_POST['mercursomedicina3'][($i-1)];
									}else{
										$mercursomedicina3 = $dadosMedicina3[($i-1)];
									}
									
									echo $db->pegaUm("SELECT crmdsccurso from maismedicomec.cursomedicina where crmid = {$mercursomedicina3['crmid']}");
									
					                //$db->monta_combo('mercursomedicina3[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursomedicina3', '', $mercursomedicina3, 'Curso de medicina');
					                
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursomedicina3\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctcursomedicina3" id="ctcursomedicina3" value="<?=$ctcursomedicina3?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Curso área de saúde: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada3){
				                	$sql = "select casid from maismedicomec.mantenedoraareasaude 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada3}
				                			order by masid";
				                	$dadosSaude3 = $db->carregar($sql);
				                	if(!$dadosSaude3){
				                		$totalSaude3 = 0;
				                	}else{
				                		$totalSaude3 = count($dadosSaude3);
				                	}
				                }else{
				                	$totalSaude3 = 0;
				                	$_POST['ctcursosaude3'] = 0;
				                }
				                
								$ctcursosaude3 = $_POST['ctcursosaude3'] ? $_POST['ctcursosaude3'] : $totalSaude3;
								
								if($ctcursosaude3==0) echo 'Nenhum curso adicionado.';
								
								for($i=1; $i<=$ctcursosaude3; $i++){
									/*
									$sql = "
					                        SELECT  m.casid AS codigo,
					                                m.cascodcurso||' - '||m.casdsccurso AS descricao
					                        FROM maismedicomec.cursoareasaude AS m
					                        JOIN maismedicomec.mantidamunicipio AS mt ON mt.muncod = m.muncod
					                        WHERE mt.mntid = {$_SESSION['maismedicomec']['mntid']}
					                        ORDER BY descricao
					                    ";
									*/
									
									if($mermantidaindicada3){
										$sql = "SELECT  min(casid) AS codigo, m.casdsccurso AS descricao
						                        FROM maismedicomec.cursoareasaude AS m
						                        WHERE m.iesid = {$mermantidaindicada3} 
						                        group by m.casdsccurso
						                        ORDER BY 2";
									}
									
									$mercursosaude3 = "";
									if($_POST['submite']){
										$mercursosaude3 = $_POST['mercursosaude3'][($i-1)];
									}else{
										$mercursosaude3 = $dadosSaude3[($i-1)];
									}
									
									echo $db->pegaUm("SELECT casdsccurso from maismedicomec.cursoareasaude where casid = {$mercursosaude3['casid']}");
									
					                //$db->monta_combo('mercursosaude3[]', $sql, 'S', "Selecione...", '', '', '', 390, 'N', 'mercursosaude3', '', $mercursosaude3, 'Curso de saúde');
					                
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctcursosaude3\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctcursosaude3" id="ctcursosaude3" value="<?=$ctcursosaude3?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Programa residência: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada3){
				                	$sql = "select mprdsc from maismedicomec.mantenedoraprogresidencia 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada3}
				                			order by mprid";
				                	$dadosResidencia3 = $db->carregarColuna($sql);
				                	if(!$dadosResidencia3){
				                		$totalResidencia3 = 0;
				                	}else{
				                		$totalResidencia3 = count($dadosResidencia3);
				                	}
				                }else{
				                	$totalResidencia3 = 0;
				                	$_POST['ctresidencia3'] = 0;
				                }
				                
								$ctresidencia3 = $_POST['ctresidencia3'] ? $_POST['ctresidencia3'] : $totalResidencia3;
								
								if($ctresidencia3==0) echo 'Nenhum programa adicionado.';
								
								for($i=1; $i<=$ctresidencia3; $i++){
									
									$merresidencia3 = "";
									if($_POST['submite']){
										$merresidencia3 = $_POST['merresidencia3'][($i-1)];
									}else{
										$merresidencia3 = $dadosResidencia3[($i-1)];
									}
									
									echo $merresidencia3;
									
					                //echo campo_texto('merresidencia3[]', 'N', 'S', 'Programa Residência', 59, 100, '', '', '', '', 0, 'id="merresidencia3"', '', $merresidencia3, '', null);
					                 
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctresidencia3\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctresidencia3" id="ctresidencia3" value="<?=$ctresidencia3?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Mestrado/doutorado: </td>
				            <td colspan="2">
				                <?
								if($mermantidaindicada3){
				                	$sql = "select mpmdsc from maismedicomec.mantenedoraprogmestdoutorado 
				                			where mntid = {$_SESSION['maismedicomec']['mntid']}
				                			and mtdid = {$mermantidaindicada3}
				                			order by mpmid";
				                	$dadosMestrado3 = $db->carregarColuna($sql);
				                	if(!$dadosMestrado3){
				                		$totalMestrado3 = 0;
				                	}else{
				                		$totalMestrado3 = count($dadosMestrado3);
				                	}
				                }else{
				                	$totalMestrado3 = 0;
				                	$_POST['ctmestrado3'] = 0;
				                }
				                
								$ctmestrado3 = $_POST['ctmestrado3'] ? $_POST['ctmestrado3'] : $totalMestrado3;
								
								if($ctmestrado3==0) echo 'Nenhum mestrado/doutorado adicionado.';
								
								for($i=1; $i<=$ctmestrado3; $i++){
									
									$mermestrado3 = "";
									if($_POST['submite']){
										$mermestrado3 = $_POST['mermestrado3'][($i-1)];
									}else{
										$mermestrado3 = $dadosMestrado3[($i-1)];
									}
									
									echo $mermestrado3;
									
					                //echo campo_texto('mermestrado3[]', 'N', 'S', 'Programa mestrado/doutorado', 59, 100, '', '', '', '', 0, 'id="mermestrado3"', '', $mermestrado3, '', null);
				
					                //echo '<img src="../includes/layout/areia/img/bt_logoff.png" border="0" style="vertical-align: middle; cursor: hand" onclick="removeCampo(\'ctmestrado3\');">';
					                
					                echo '<br>';
									
								}
				                
				                ?>
				                <input type="hidden" name="ctmestrado3" id="ctmestrado3" value="<?=$ctmestrado3?>">
				            </td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita" colspan="2">&nbsp;</td>
				        </tr>
				        <tr>
				            <td class ="SubTituloDireita">Existência de supervisão institucional nos últimos dois anos? </td>
				            <td colspan="2">
				                <?
				                	if($merinexistsupervinstitucional == 't'){
				                		echo 'SIM';
				                	}else{
				                		echo 'NÃO';
				                	}
				                ?>
				            </td>
				        </tr>
				    </table>
                    
                </td>
            </tr>
			<tr>
                <td class="subTituloCentro" colspan="3" style="text-align: left; text-transform:uppercase;"> Portfólio Mantida </td>
            </tr>
            <tr>
                <td colspan="3" >
                    
                    <?
						$sql = "SELECT  m.mtddsc,
						                crmdsccurso,
						                mneprogresidencia1, 
						                mneprogresidencia2, 
									    mneprogresidencia3, 
									    mneprogresidencia4, 
									    mneprogresidencia5, 
									    mneprogmestdoutorado1, 
									    mneprogmestdoutorado2, 
									    mneprogmestdoutorado3, 
									    mneprogmestdoutorado4, 
									    mneprogmestdoutorado5,
						                cs1.casdsccurso as cursosaude1,
						                cs2.casdsccurso as cursosaude2,
						                cs3.casdsccurso as cursosaude3,
						                case when mneaderentefies = 't' then 
						                		'SIM'
						                	 else
						                	 	'NÃO'
						                end as fies,
						                case when mneaderenteprouni = 't' then 
						                		'SIM'
						                	 else
						                	 	'NÃO'
						                end as prouni
						        FROM maismedicomec.mantidaexpregulatoria me
						        JOIN maismedicomec.mantida AS m ON m.mtdid = me.mtdid
						        left JOIN maismedicomec.cursomedicina AS cm ON cm.crmid = me.mnecursomedicina
						        left JOIN maismedicomec.cursoareasaude AS cs1 ON cs1.casid = me.mnecursoareasaude1
						        left JOIN maismedicomec.cursoareasaude AS cs2 ON cs2.casid = me.mnecursoareasaude2
						        left JOIN maismedicomec.cursoareasaude AS cs3 ON cs3.casid = me.mnecursoareasaude3
						        WHERE mnestatus = 'A' AND mntid = {$_SESSION['maismedicomec']['mntid']}";
						    	/*
						        if(in_array(PERFIL_HOSPITAL, $perfil)){            
						            $sql .= " and pe.pflcod = ".PERFIL_HOSPITAL;
						            $sql .= " and m.usucpf = '{$_SESSION['usucpf']}'";
						        }
						        
						        if(in_array(PERFIL_IES, $perfil)){            
						            $sql .= " and pe.pflcod = ".PERFIL_IES;
						            $sql .= " and m.usucpf = '{$_SESSION['usucpf']}'";
						        }
								*/
						        
							
						    $sql .=   " ORDER BY m.mtddsc";
						    //ver($sql,d);
						    $cabecalho = array("Mantida", "C.M.", "P.R.1", "P.R.2", "P.R.3", "P.R.4", "P.R.5", "P.M.1", "P.M.2", "P.M.3", "P.M.4", "P.M.5", "C.S.1", "C.S.2", "C.S.3", "FIES", "PROUNI");
						    $alinhamento = Array('left');
						    //$tamanho = Array('4%');
						    $db->monta_lista($sql, $cabecalho, 1000, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);

                    ?>
                    
                </td>
            </tr>
    </table>

	<div class="notprint">
	    <table align="center" bgcolor="#f5f5f5" border="0" cellpadding="3" cellspacing="1">
	        <tr>
	            <td class="SubTituloCentro" colspan="4" style="font-weight: bold">
	                
	              <input type="button" id="imprimir" value="Imprimir" onclick="self.print();"/>
	              
	              
	            </td>
	        </tr>
	    </table>
    </div>
</form>


