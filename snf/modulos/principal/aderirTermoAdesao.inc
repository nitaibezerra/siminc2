<?php
if($_REQUEST['requisicao']){
	$_REQUEST['requisicao']();
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";


$insid  = isset($_GET['insid']) ? $_GET['insid'] : $_SESSION['snf']['insid'];
$usucpf = $_SESSION['usucpf'];
?>
<br />
<?php $db->cria_aba($abacod_tela, $url, ''); ?>
<?php monta_titulo('Adesão à Rede Nacional de Formação Continuada dos Profissionais do Magistério da Educação Básica', obrigatorio() . "Indica campos obrigatórios."); ?>
<link href="../snf/geral/snf.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>

		<script type="text/javascript" src="../includes/prototype.js"></script>
		<script type="text/javascript" src="../includes/funcoes.js" ></script>
		<script type="text/javascript" src="../includes/entidadesn.js"></script>
		<!--<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>-->		
		<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
		<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
		<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
    
    
<?php
if (verificaPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_ADMINISTRADOR))) {
?> 
    	<script>
                function selecionaInstituicao(insid) {
                    location.href = '?modulo=principal/aderirTermoAdesao&acao=A&insid=' + insid;
                }
    	</script>
    	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    		<tr>
    			<td colspan="2" class="SubtituloTabela bold" >Selecione uma Instituição para Visualizar os Dados</td>
    		</tr>
    		<tr class="center SubtituloDireita" >
    			<td colspan="2">
    <?php
    $sql = "SELECT
						DISTINCT
						ie.insid AS codigo,
						ie.insnome AS descricao
					FROM
						snf.dirigentemaximo dir
					JOIN
						snf.instituicaoensino ie ON ie.insid = dir.insid AND
													ie.insstatus = 'A'
					WHERE
						dir.dirstatus = 'A'
					ORDER BY
						descricao";
    $db->monta_combo("insid", $sql, "S", "Selecione uma Instituição...", "selecionaInstituicao", "", "", '', "N");
    ?>	
    			</td>
    		</tr>
    	</table>
    <?php
    if (empty($insid)) {
        die("<script>jQuery('#aguarde').hide();</script>");
    }
}

if ($insid) {
    $arrDados = pegaDadosInstituicaoPorId($insid);
} else {
    $arrDados = pegaDadosInstituicao($usucpf);
}

if (!$arrDados) {
    unset($_SESSION['snf']['insid']);
    naoExisteDirigente();
    die("<script>jQuery('#aguarde').hide();</script>");
} else {
    $_SESSION['snf']['insid'] = $arrDados['insid'];
}

$regra = verificaRegrasGerarTermo();
    ?>
<script type="text/javascript">
	jQuery.noConflict();
        
        // Contorno para resolver o conflito entre Chrome + mascara + onchange + jquery
        var lastCPF = new Array();
	
	jQuery(function() {
		jQuery("[name^=memcpf]").each(function() { 
                    
                    // Contorno do campo 'memcpf' para resolver o conflito entre Chrome + mascara + onchange + jquery
                    jQuery("#" + this.id ).blur(function(){
                        if( (lastCPF[this.id] == 'undefined') || (lastCPF[this.id] != jQuery("#" + this.id ).val()) ){
                           lastCPF[this.id] = jQuery("#" + this.id ).val();
                           verificaCPFRF(this);
                        }
                    });
                    
                    jQuery("#" + this.id ).val( mascaraglobal("###.###.###-##",jQuery("#" + this.id ).val()) );
                        
		});
                
                
                
	});
        
	function excluirArquivoComite(arqid)
	{
		if(confirm("Deseja realmente excluir o documento?")){
			jQuery.ajax({
					   type: "POST",
					   url: 'snf.php?modulo=principal/aderirTermoAdesao&acao=A',
					   data: "requisicao=excluirArquivo&arqid=" + arqid,
					   success: function(msg){
					   		jQuery("#div_file_link").remove();
					   		jQuery("#div_file_upload").show();
					   }
					 });
		}
	}
	function excluirArquivoComite2(arqid)
	{
		if(confirm("Deseja realmente excluir o documento?")){
			jQuery.ajax({
					   type: "POST",
					   url: 'snf.php?modulo=principal/aderirTermoAdesao&acao=A',
					   data: "requisicao=excluirArquivo&arqid=" + arqid,
					   success: function(msg){
					   		jQuery("#div_file_link2").remove();
					   		jQuery("#div_file_upload2").show();
					   }
					 });
		}
	}
	
	function excluirArquivoDirigente(arqid)
	{
		if(confirm("Deseja realmente excluir o documento?")){
			jQuery.ajax({
					   type: "POST",
					   url: 'snf.php?modulo=principal/aderirTermoAdesao&acao=A',
					   data: "requisicao=excluirArquivo&arqid=" + arqid,
					   success: function(msg){
					   		jQuery("#div_file_dirigente_link").remove();
					   		jQuery("#div_file_dirigente_upload").show();
					   }
					 });
		}
	}
	
	function downloadArquivo(arqid)
	{
		window.location.href='snf.php?modulo=principal/aderirTermoAdesao&acao=A&requisicao=downloadArquivo&arqid=' + arqid;
	}
	
	function verificaCPFRF(obj)
	{
		if(obj.value){
			jQuery.ajax({
                            type: "POST",
                            url: 'snf.php?modulo=principal/aderirTermoAdesao&acao=A',
                            data: "requisicao=verificaCPFRF&cpf=" + obj.value,
                            success: function(msg){
                                         if(msg.search("Nome:") >= 0){
                                                 jQuery("#" + obj.id.replace("memcpf","memnome") ).val(msg.replace("Nome:",""));
                                         }else{
                                                 alert(msg);
                                                 jQuery("#" + obj.id.replace("memcpf","memnome") ).val("");
                                         }
                }
            });
        } else {
            jQuery("#" + obj.id.replace("memcpf", "memnome")).val("");
        }
    }

    function filtraCidade(estuf)
    {
        jQuery.ajax({
            type: "POST",
            url: 'snf.php?modulo=principal/aderirTermoAdesao&acao=A',
            data: "requisicao=filtraCidade&estuf=" + estuf,
            success: function(msg) {
                jQuery("#td_muncod").html(msg);
            }
        });
    }

    function filtraCidadeEndereco(estuf)
    {
        jQuery.ajax({
            type: "POST",
            url: 'snf.php?modulo=principal/aderirTermoAdesao&acao=A',
            data: "requisicao=filtraCidade&estuf=" + estuf + "&name=muncod_ende",
            success: function(msg) {
                jQuery("#td_muncod_endereco").html(msg);
            }
        });
    }

    function addNovoMembro()
    {
        var numero = (jQuery("[name='memcpf[]']").length) + 1;

        for (i = 1; i <= numero; i++)
        {
            if (!jQuery("#memcpf_" + i).attr("name")) {
                var novoNumero = i;
                break;
            }
        }

        jQuery.ajax({
            type: "POST",
            url: 'snf.php?modulo=principal/aderirTermoAdesao&acao=A',
            data: "requisicao=htmlAddNovoMembro&numero=" + novoNumero,
            success: function(msg) {
                jQuery("table#tabela_comite tr").eq((jQuery("table#tabela_comite tr").length) - 3).after(msg);
            }
        });

    }

    function excluiMembroComite(obj)
    {
        var linha = jQuery(obj).parent().parent().attr("rowIndex");
        jQuery("table#tabela_comite tr").eq(linha).remove();
        jQuery("table#tabela_comite tr").eq(linha).remove();
        jQuery("table#tabela_comite tr").eq(linha).remove();
        jQuery("table#tabela_comite tr").eq(linha).remove();
        jQuery("table#tabela_comite tr").eq(linha).remove();

    }

    function salvarComiteGestor(continuar)
    {

        var erro = 0;
        jQuery("[class~=obrigatorio]").each(function() {
            if (!this.value || this.value == "Selecione...") {
                element = jQuery(this);

                if (element.attr('name') == 'memobs[]') {

                    // Validando campo texto outros somente se no selec papel estiver selecionado outros.
                    if (element.parent().parent('').prev('').children('').children('').val() == 8) {
                        erro = 1;
                        alert('Favor preencher todos os campos obrigatórios.');
                        this.focus();
                        return false;
                    }
                } else {
                    erro = 1;
                    alert('Favor preencher todos os campos obrigatórios.');
                    this.focus();
                    return false;
                }
            }
        });

        /*
         if(!erro && !jQuery("[name='arquivo_dirigente']").val() && !jQuery("[name='hdn_arquivo_dirigente']").val() ){
         erro = 1;
         alert('Favor inserir o documento do ato de nomeação do dirigente.');
         return false;
         }
         */



        if (erro == 0) {
            jQuery("[name='continuar']").val(continuar);
            jQuery("[name='requisicao']").val("salvarComite");
            jQuery("[name='formulario_comite']").submit();
        }
    }

    function preenchimentoOk()
    {
        var regra = '<?php echo $regra; ?>';

        if (!jQuery("[name='arquivo_comite']").val() && !jQuery("[name='hdn_arquivo_comite']").val()) {
            alert('É necessário anexar o Ato Constitutivo que cria o Comitê Institucional.');
            return false;
        }

        if (regra == '') {
            salvarComiteGestor(1);
        }
        else {
            alert(regra);
        }
    }
    
</script>
<form name="formulario_comite" id="formulario_comite"  method="post" action="" enctype="multipart/form-data" >
    <input type="hidden"  name="requisicao" value="" />
    <input type="hidden"  name="continuar" value="" />
    <input type="hidden"  name="insid" value="<?php echo $arrDados['insid'] ?>" />
    <input type="hidden"  name="dirid" value="<?php echo $arrDados['dirid'] ?>" />
    <table id="tabela_comite" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td colspan="2" class="SubtituloTabela bold" >Dados da Instituição</td>
        </tr>
        <tr>
            <td width="25%" class="SubtituloDireita" >Nome:</td>
            <td>
                <?php echo campo_texto("insnome", "S", "S", "", 60, 255, "", "", "", "", "", "", "", $arrDados['insnome']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Sigla:</td>
            <td>
                <?php echo campo_texto("inssigla", "N", "S", "", 40, 50, "", "", "", "", "", "", "", $arrDados['inssigla']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >CNPJ:</td>
            <td>
                <?php echo campo_texto("inscnpj", "S", "S", "", 40, 20, "##.###.###/####-##", "", "", "", "", "", "", ($arrDados['inscnpj'] ? mascaraglobalTermoAdesao($arrDados['inscnpj'], "##.###.###/####-##") : "")) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Telefone:</td>
            <td>
                <?php echo campo_texto("instelefone", "S", "S", "", 40, 50, "", "", "", "", "", "", "", $arrDados['instelefone']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Fax:</td>
            <td>
                <?php echo campo_texto("insfax", "N", "S", "", 40, 50, "", "", "", "", "", "", "", $arrDados['insfax']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >E-mail:</td>
            <td>
                <?php echo campo_texto("insemail", "N", "S", "", 40, 50, "", "", "", "", "", "", "", $arrDados['insemail']) ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="SubtituloTabela bold" >Endereço</td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >CEP:</td>
            <td>
                <input type="hidden" name="endid" value="<?php echo $arrDados['endid'] ?>" />
                <?php echo campo_texto("endcep", "N", "S", "", 11, 13, "##.###-###", "", "", "", "", "id='endcep1'", "", ($arrDados['endcep'] ? mascaraglobalTermoAdesao($arrDados['endcep'], "##.###-###") : ""), '') ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Logradouro:</td>
            <td>
                <?php echo campo_texto("endlog", "N", "S", "", 60, 255, "", "", "", "", "", "id='endlog1'", "", $arrDados['endlog']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Complemento:</td>
            <td>
                <?php echo campo_texto("endcom", "N", "S", "", 60, 255, "", "", "", "", "", "id='endcom1'", "", $arrDados['endcom']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Número:</td>
            <td>
                <?php echo campo_texto("endnum", "N", "S", "", 40, 50, "", "", "", "", "", "id='endnum1'", "", $arrDados['endnum']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Bairro:</td>
            <td>
                <?php echo campo_texto("endbai", "N", "S", "", 60, 255, "", "", "", "", "", "id='endbai1'", "", $arrDados['endbai']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Estado:</td>
            <td>
                <?php $sqlUF = "select 
                                        estuf as codigo,
                                        estdescricao as descricao 
                                from 
                                        territorios.estado
                                order by 
                                        estdescricao"; ?>
                <?php $db->monta_combo("estuf_ende", $sqlUF, "S", "Selecione...", "filtraCidadeEndereco", "", "", '', "S", "estuf1", "", $arrDados['estuf']) ?>
                <?php // $db->monta_combo("teste",$sqlUF,"S","Selecione...","","","",'',"S","estuf1","",$arrDados['estuf']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Município:</td>
            <td id="td_muncod_endereco" >
                <?php
                if ($arrDados['estuf']) {
                    $disponivel = "S";
                    $sqlMun = "	select 
				  muncod as codigo,
				  mundescricao as descricao 
				from 
				  territorios.municipio
				where
				  estuf = '{$arrDados['estuf']}'
				order by 
				  mundescricao";
                } else {
                    $disponivel = "N";
                    $sqlMun = array();
                }
                ?>

<?php $db->monta_combo("muncod_ende", $sqlMun, $disponivel, "Selecione...", "", "", "", 200, "S", "muncod1", "", $arrDados['muncod']) ?>
            </td>
        </tr>
        <tr>
            <td colspan="2" class="SubtituloTabela bold" >Dados do Dirigente</td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Nome:</td>
            <td><?php echo $arrDados['dirnome'] ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >CPF:</td>
            <td><?php echo $arrDados['dircpf'] ? mascaraglobalTermoAdesao($arrDados['dircpf'], "###.###.###-##") : "N/A" ?></td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >Cargo:</td>
            <td>
                <?php $sqlCargo = "	select 
										carid as codigo, 
										cardescricao as descricao 
									from 
										snf.cargo 
									where 
										carstatus = 'A'
									order by 
										cardescricao"; ?>
<?php $db->monta_combo("carid", $sqlCargo, "S", "Selecione...", "", "", "", 200, "S", "", "", $arrDados['carid']) ?>
            </td>
        </tr>
        <!--
        <tr>
                <td class="SubtituloDireita" >Ato de Nomeação:</td>
                <td>
<?php if ($arrDados['arqid']): ?>
                                    <div id="div_file_dirigente_link" >
                                            <a href='javascript:downloadArquivo(<?php echo $arrDados['arqid'] ?>)' ><?php echo $arrDados['anexo_dirigente'] ?></a> <img src='../imagens/excluir.gif' onclick='excluirArquivoDirigente(<?php echo $arrDados['arqid'] ?>)' style='vertical-align:middle;cursor:pointer' />
                                            <input type="hidden" name="hdn_arquivo_dirigente" value="1"  />
                                    </div>
    <?php $ocultaFileDirigente = true ?>
<?php endif; ?>
                        <div id="div_file_dirigente_upload" style="display:<?php echo $ocultaFileDirigente ? "none" : "" ?>" >
                                <input type="file" name="arquivo_dirigente"  /> <?php echo obrigatorio() ?><span style="padding-left:10px" >* Documento do Ato de Nomeação do Dirigente.</span>
                        </div>
                </td>
        </tr>
        -->
        <tr>
            <td colspan="2" class="SubtituloTabela bold" >Dados do Comitê Institucional</td>
        </tr>
        <tr class="SubtituloCentro" >
            <td colspan="2" class="bold" >Coordenador Institucional</td>
        </tr>
        <?php
        $sql = "	select
							*
						from
							snf.comite com
						left join
							snf.anexo anx ON anx.aneid = com.aneid2 and anestatus = 'A'
						left join
							public.arquivo arq ON anx.arqid = arq.arqid
						left join
							snf.membrocomite mem ON com.comid = mem.comid and memstatus = 'A'
						where
							com.insid = {$arrDados['insid']}
						and
							comstatus = 'A'";
        $arrDados2 = $db->carregar($sql);

        if ($arrDados2) {
            foreach ($arrDados2 as $dado2) {
                if ($dado2['papid'] == PAPEL_COORD_INSTITUCIONAL) {
                    $arrCood2 = $dado2;
                } else {
                    $arrMembros2[] = $dado2;
                }
            }
        }


        $sql = "	select
							*
						from
							snf.comite com
						left join
							snf.anexo anx ON anx.aneid = com.aneid and anestatus = 'A'
						left join
							public.arquivo arq ON anx.arqid = arq.arqid
						left join
							snf.membrocomite mem ON com.comid = mem.comid and memstatus = 'A'
						where
							com.insid = {$arrDados['insid']}
						and
							comstatus = 'A'";
        $arrDados = $db->carregar($sql);

        if ($arrDados) {
            foreach ($arrDados as $dado) {
                if ($dado['papid'] == PAPEL_COORD_INSTITUCIONAL) {
                    $arrCood = $dado;
                } else {
                    $arrMembros[] = $dado;
                }
            }
        }

//                        ver($sql, $arrDados2,d)
        ?>
        <tr>
            <td class="SubtituloDireita" width="25%">
                CPF:
            </td>
            <td>
                <input type="hidden"  name="papid[]" value="<?php echo PAPEL_COORD_INSTITUCIONAL ?>" />
<?php echo campo_texto("memcpf[]", "S", "S", "", 18, 14, "###.###.###-##", "", "", "", "", " id='memcpf_1' onchange='verificaCPFRF(this)' ", "", $arrCood['memcpf']) ?>
                <span style="padding-left:10px" >* Coordenador institucional é o coordenador geral indicado pelo Reitor, conforme § 4º, artigo 1º da <a target="_blank" title="Portaria 1.328"  href="../snf/geral/resolucao_1_2011.pdf"  >Resolução nº 1</a> de 17/08/2011.</span>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >
                Nome:
            </td>
            <td>
<?php echo campo_texto("memnome[]", "S", "N", "", 60, 255, "", "", "", "", "", " id='memnome_1' ", "", $arrCood['memnome']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >
                Email:
            </td>
            <td>
<?php echo campo_texto("mememail", "S", "S", "", 60, 255, "", "", "", "", "", "", "", $arrCood['mememail']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >
                Telefone:
            </td>
            <td>
<?php echo campo_texto("memtelefone", "S", "S", "", 18, 20, "", "", "", "", "", "", "", $arrCood['memtelefone']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >
                Endereço:
            </td>
            <td>
<?php echo campo_texto("memendereco", "S", "S", "", 80, 255, "", "", "", "", "", "", "", $arrCood['memendereco']) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >
                Estado:
            </td>
            <td>
                <?php $estuf = $arrCood['estuf'] ?>
                <?php $sql = "select
								estuf as codigo,
								estdescricao as descricao
							from
								territorios.estado
							order by
								estdescricao" ?>
<?php $db->monta_combo("estuf", $sql, "S", "Selecione...", "filtraCidade", "", "", "", "S", "", "", $estuf) ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >
                Município:
            </td>
            <td id="td_muncod" >
                <?php $muncod = $arrCood['muncod'] ?>
                <?php
                if (!$estuf) {
                    $db->monta_combo("muncod", $sql, "N", "Selecione...", "", "", "", "200", "S");
                } else {
                    $sql = "select
					   				muncod as codigo,
					   				mundescricao as descricao
					   			from
					   				territorios.municipio
					   			where
					   				estuf = '$estuf'
					   			order by
					   				mundescricao";
                    $db->monta_combo("muncod", $sql, "S", "Selecione...", "", "", "", "200", "S", "", "id='muncod'", $muncod);
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >
                Ato Constitutivo:
            </td>
            <td>
                    <?php //if($arrCood['arqid']):  ?>
                <div id="div_file_link" >
<?php if ($arrCood['arqid']): ?>
                        <a href='javascript:downloadArquivo(<?php echo $arrCood['arqid'] ?>)' ><?php echo $arrCood['arqnome'] ?>.<?php echo $arrCood['arqextensao'] ?></a> <img src='../imagens/excluir.gif' onclick='excluirArquivoComite(<?php echo $arrCood['arqid'] ?>)' style='vertical-align:middle;cursor:pointer' />
                <?php endif ?>
                    <input type="hidden" name="hdn_arquivo_comite" value="<?php echo $arrCood['arqnome'] ?>"  />
                </div>
<?php //$ocultaFile = true  ?>
<?php //endif;  ?>
                <div id="div_file_upload" style="display:<?php echo $ocultaFile ? "none" : "" ?>" >
                    <input type="file" name="arquivo_comite"  /> <?php echo obrigatorio() ?><span style="padding-left:10px" >*Documentos que comprem a criação do Comitê Institucional e a designação do Coordenador Institucional.</span> <br />
                </div>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita" >
                Nomeação do Coordenador Institucional:
            </td>
            <td>
                    <?php //if($arrCood['arqid']):  ?>
                <div id="div_file_link2" >
<?php if ($arrCood2['arqid']): ?>
                        <a href='javascript:downloadArquivo(<?php echo $arrCood2['arqid'] ?>)' ><?php echo $arrCood2['arqnome'] ?>.<?php echo $arrCood2['arqextensao'] ?></a> <img src='../imagens/excluir.gif' onclick='excluirArquivoComite2(<?php echo $arrCood2['arqid'] ?>)' style='vertical-align:middle;cursor:pointer' />
                <?php endif ?>
                    <input type="hidden" name="hdn_arquivo_comite2" value="<?php echo $arrCood2['arqnome'] ?>"  />
                </div>
<?php //$ocultaFile = true  ?>
<?php //endif;  ?>
                <div id="div_file_upload2" style="display:<?php echo $ocultaFile ? "none" : "" ?>" >
                    <input type="file" name="arquivo_comite2"  /> <?php echo obrigatorio() ?><span style="padding-left:10px" ></span>
                </div>
            </td>
        </tr>
        <tr class="SubtituloCentro" >
            <td colspan="2" class="bold" >Demais Membros do Comitê</td>
        </tr>
<?php if ($arrMembros): ?>
    <?php $n = 2; ?>
    <?php foreach ($arrMembros as $membro): ?>
                <tr class="SubtituloTabela" >
                    <td colspan="2" >
                        Membro <?php echo ($n - 1) ?> 
                        <!--<img onclick="excluiMembroComite(this)" style="background-color:#FFFFFF" class="img_middle link" src="../imagens/excluir.gif">-->
                        <img src="../imagens/excluir.gif" class="img_middle link" style="background-color:#FFFFFF" onclick="excluiMembroComite(this)"   />
                    </td>
                </tr>
                <tr>
                    <td class="SubtituloDireita">
                        CPF:
                    </td>
                    <td>
        <?php echo campo_texto("memcpf[]", "S", "S", "", 18, 14, "###.###.###-##", "", "", "", "", " id='memcpf_$n' onchange='verificaCPFRF(this)' ", "", $membro['memcpf']) ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubtituloDireita" >
                        Nome:
                    </td>
                    <td>
        <?php echo campo_texto("memnome[]", "S", "N", "", 60, 255, "", "", "", "", "", " id='memnome_$n' ", "", $membro['memnome']) ?>
                    </td>
                </tr>
                <tr>
                    <td class="SubtituloDireita" >
                        Papel:
                    </td>
                    <td>
                        <?php $sql = "select
										papid as codigo,
										papdescricao as descricao
									from
										snf.papelcomite
									where
										papstatus = 'A'
									order by
										papdescricao" ?>
        <?php $db->monta_combo("papid[]", $sql, "S", "Selecione...", "mostrarOutros(this)", "", "", "200", "S", "", "", $membro['papid']) ?>
                    </td>
                </tr>
                <tr class="tr_outros" <?php if ($membro['papid'] != 8) echo 'style="display: none;"' ?> >
                    <td class="SubtituloDireita" >
                        Outros:
                    </td>
                    <td>
                <?php echo campo_texto("memobs[]", "S", "S", "", 30, 50, "", "", "", "", "", "", "", $membro['memobs']) ?>
                    </td>
                </tr>
        <?php $n++; ?>
    <?php endforeach; ?>
<?php else: ?>
            <tr>
                <td class="SubtituloDireita" >
                    CPF:
                </td>
                <td>
    <?php echo campo_texto("memcpf[]", "S", "S", "", 18, 14, "###.###.###-##", "", "", "", "", " id='memcpf_2' onchange='verificaCPFRF(this)' ") ?>
                </td>
            </tr>
            <tr>
                <td class="SubtituloDireita" >
                    Nome:
                </td>
                <td>
    <?php echo campo_texto("memnome[]", "S", "N", "", 60, 255, "", "", "", "", "", " id='memnome_2' ") ?>
                </td>
            </tr>
            <tr>
                <td class="SubtituloDireita" >
                    Papel:
                </td>
                <td>
                    <?php $sql = "select
									papid as codigo,
									papdescricao as descricao
								from
									snf.papelcomite
								where
									papstatus = 'A'
								order by
									papdescricao" ?>
    <?php $db->monta_combo("papid[]", $sql, "S", "Selecione...", "mostrarOutros(this)", "", "", "200", "S", "", "", '') ?>
                </td>
            </tr>
            <tr class="tr_outros" style="display: none;">
                <td class="SubtituloDireita" >
                    Outros:
                </td>
                <td>
            <?php echo campo_texto("memobs[]", "S", "S", "", 30, 50, "", "", "", "", "", "") ?>
                </td>
            </tr>
<?php endif; ?>
        <tr class="SubtituloTabela" >
            <td colspan="2" >
                    <!--<img onclick="addNovoMembro()" src="../imagens/gif_inclui.gif" class="img_middle link"  /> <a href="javascript:addNovoMembro()" >Adicionar Novo Membro</a>-->
                <a onclick="addNovoMembro();">Adicionar Novo Membro</a>
            </td>
        </tr>
        <?php
        $sql = "select teraprovacao from snf.termoadesao where insid = {$_SESSION['snf']['insid']}";
        $teraprovacao = $db->pegaUm($sql);
        ?>
<?php //if($teraprovacao != "S"):  ?>
        <tr>
            <td class="SubtituloTabela center" colspan="2" >
                <input type="button" name="btn_salvar" onclick="salvarComiteGestor()" value="Salvar" />
                <input type="button" name="btn_salvar_continuar" value="Continuar" onclick="preenchimentoOk();" />
                <input type="button" name="btn_voltar" value="Voltar" onclick="history.back(-1)" />
            </td>
        </tr>
<?php //else:  ?>
        <script>
                //jQuery("[type='text'],select").attr("disabled","disabled");
//				jQuery("[href='javascript:addNovoMembro()'],[src='../imagens/excluir.gif'],[src='../imagens/gif_inclui.gif']").remove();
        </script>
<?php //endif;  ?>
    </table>
</form>


<script language="javascript">
    
//    jQuery('select[name=papid[]]').change(function(){
////        jQuery('select[name=papid[]]').val()
//        console.info(this);
//        element = jQuery(this);
//        
//        if(element.val() == 8){
//            
//            element.parent().parent('tr').next('.tr_outros').fadeIn('slow');
////            alert(jQuery(this).val());
//        } else {
//            element.parent().parent('tr').next('.tr_outros').fadeOut('slow');
//        }
//        
//    });
    
    function mostrarOutros(element)
    {
        element = jQuery(element);
        if(element.val() == 8){
            element.parent().parent('tr').next('.tr_outros').fadeIn('slow');
        } else {
            element.parent().parent('tr').next('.tr_outros').fadeOut('slow');
        }
    }
//            alert(jQuery(this).val());
    
    
//    function MouseBlur(element)
//    {
////        element = jQuery(element);
//        
//        if(jQuery(element).attr('name') == 'endcep'){
////            console.info(element.val());
////            getEnderecoPeloCEP(element.value,'1');
//alert(element.value);
//        }
        
//    }
//    getEnderecoPeloCEP(this.value,'".$this->_listatipoendereco[$tendereco]['tpeid']."')
    
//    function getCEP(element){
//        getEnderecoPeloCEP(jQuery(this).val(),'1');
//    }
    
    // Função AJAX que pega os dados do endereço atraves do CEP
    // as informações são retornadas com o separador "||"
    jQuery('#endcep1').blur(function(){
        if(jQuery(this).val() != ''){
//        01503-000
        jQuery.post( "/geral/consultadadosentidade.php?requisicao=pegarenderecoPorCEP", {endcep : jQuery(this).val()},function( data ) {
            console.info(data);
            var dados = data.split("||");
//            document.getElementById('mundescricao1').value = dados[2];
                setTimeout(function(){
                        document.getElementById('endbai1').value = dados[1];
                        document.getElementById('endlog1').value = dados[0];
                        jQuery('#estuf1').val(dados[3]);
                        filtraCidadeEndereco(dados[3])
                }, 300);
                setTimeout(function(){
                        jQuery('#muncod1').val(dados[4]);
                }, 400);
                
          });
        }
    });
</script>