<?php
 
$docid = evtCriarDoc();

if($_REQUEST['download'] == 'S'){
	/*
    $param = array();
    $param["arqid"] = $_REQUEST['arqid'];
    DownloadArquivo($param);
    exit;
    */
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['arqid'];
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = '".$_SERVER['HTTP_REFERER']."';</script>";
    exit;
    

} 

if( $_SESSION['eveid'] != '')
{ 
	$sql = "SELECT 
                        ev.evetitulo,  
                        ev.ungcod,
                        ev.tpeid,			 
                        ev.evedatainicio,		 
                        ev.evedatafim,			 
                        ev.everespnome,
                        ev.eveemail, 		 
                        ev.evenumeropi, 		 
                        ev.evenumeroprocesso,   
                        ev.evecustoprevisto, 	 
                        ev.evepublicoestimado,  
                        ev.muncod, 				 
                        ev.estuf, 				 
                        ev.sevid,
                        ev.eveurgente,
                        u.ungdsc,
                        us.usunome,
                        ev.docid,
                        to_char(ev.evedatainclusao::date,'DD/MM/YYYY') AS evedatainclusao	 
                FROM 
                        evento.evento AS ev
                LEFT JOIN evento.tipoevento AS te 
                        ON te.tpeid = ev.tpeid
                LEFT JOIN 
                        public.unidadegestora AS u ON ev.ungcod = u.ungcod
                LEFT JOIN seguranca.usuario AS us ON us.usucpf = ev.usucpf 
                WHERE
                        ev.eveid = '".$_SESSION['eveid']."'";
	
	$rsDadosEvento = $db->carregar( $sql );
	 
}

//insere dados do orçamento resultado
if($_REQUEST['salvar'] == '1'){
	/*
    $anexos = array();
    $anexos['sql'] = "INSERT INTO 
    					evento.anexoevento (eveid,
    										tpaid,
    										arqid, 
    										axestatus, 
    										axedatainclusao, 
    										usucpf) 
    					VALUES( ".$_SESSION["eveid"].",
    							".$_REQUEST["tpaid"].", 
    							#arqid#,
    							'A',
    							'".date("Y-m-d")."',
    							".$_SESSION['usucpf']." 
    							)";
    $dir = 'anexos&acao=A';

    EnviarArquivo($_FILES, $_POST, $dir, $anexos);
    exit;
	*/
	
	if($_FILES['arquivo']['error'] == 0) {
		
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
		$campos = array("eveid"         	=> $_SESSION["eveid"],
						"tpaid"         	=> $_REQUEST["tpaid"],
						"usucpf"        	=> "'".$_SESSION['usucpf']."'",
						"axedatainclusao" 	=> "NOW()",
						"axestatus"     	=> "'A'");
	
		$file = new FilesSimec("anexoevento", $campos ,"evento");
	 	$file->setUpload( (($_REQUEST['arqdescricao']) ? $_REQUEST['arqdescricao'] : NULL ), $key  = "arquivo" );
		 
	    echo"<script> alert('Operação realizada com sucesso!');</script>";
	    echo"<script>window.location.href = 'evento.php?modulo=principal/cadEventoAnexo&acao=A';</script>";
		exit;
		
	}

}


if($_REQUEST['arqidDel'] != ''){
   /*
    $anexos = array();
    $sql1 = "DELETE FROM evento.anexoevento where arqid = ".$_REQUEST['arqidDel']; 
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    //$sql = "UPDATE evento.anexoevento SET axestatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $db->executar($sql1);

    $db->commit();
*/
	
//deleta arquivo
	$arquivo = $db->pegaLinha("select arqid, axpid from evento.anexoevento where arqid = ". $_REQUEST['arqidDel']);
	if($arquivo['arqid']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$campos	= array("axpid" => $arquivo['axpid']);
		$file   = new FilesSimec("anexoevento", $campos, "evento");
		// Remove o registro do anexo antigo (pelo arqid) e o arquivo físico
		$file->setRemoveUpload( $arquivo['arqid'] );
	}
    echo"<script>window.location.href = 'evento.php?modulo=principal/cadEventoAnexo&acao=A';</script>";

}

/*
function EnviarArquivo($arquivo,$dados,$dir = 'anexos',$anexos){
    global $db;

    // obtém o arquivo
    $arquivo = $_FILES['arquivo'];

    if ( !is_uploaded_file( $arquivo['tmp_name'] ) ) {
        ?>
            <script>
                location.href='evento.php?modulo=<?=$_REQUEST['modulo']?>&acao=<?= $_REQUEST['acao']?>';
            </script>
        <?
    }
    // BUG DO IE
    // O type do arquivo vem como image/pjpeg
    if($arquivo["type"] == 'image/pjpeg') {
        $arquivo["type"] = 'image/jpeg';
    }
    //Insere o registro do arquivo na tabela public.arquivo
    $sql = "INSERT INTO public.arquivo  (arqnome,arqextensao,arqdescricao,arqtipo,arqtamanho,arqdata,arqhora,usucpf,sisid,arqstatus)
    values('".current(explode(".", $arquivo["name"]))."','".end(explode(".", $arquivo["name"]))."','".$dados["arqdescricao"]."','".$arquivo["type"]."','".$arquivo["size"]."','".date('Y-m-d')."','".date("H:i:s")."','".$_SESSION["usucpf"]."',". $_SESSION["sisid"] .",'A') RETURNING arqid;";
//    echo'<pre>';
//    print_r( $sql );
//    die();
    $arqid = $db->pegaUm($sql);

    //Insere o registro na tabela obras.arquivosobra
    $sql = str_replace("#arqid#",$arqid,$anexos['sql']);
    $db->executar($sql); 
    
    if(!is_dir('../../arquivos/evento/'.floor($arqid/1000))) {
        mkdir(APPRAIZ.'/arquivos/evento/'.floor($arqid/1000), 0777);
    }
    $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arqid/1000) .'/'. $arqid;
    switch($arquivo["type"]) {
        case 'image/jpeg':
            ini_set("memory_limit", "128M");
            list($width, $height) = getimagesize($arquivo['tmp_name']);
            $original_x = $width;
            $original_y = $height;
            // se a largura for maior que altura
            if($original_x > $original_y) {
                   $porcentagem = (100 * 640) / $original_x;     
            }else {
                   $porcentagem = (100 * 480) / $original_y; 
            }
            $tamanho_x = $original_x * ($porcentagem / 100);
            $tamanho_y = $original_y * ($porcentagem / 100);
            $image_p = imagecreatetruecolor($tamanho_x, $tamanho_y);
            $image   = imagecreatefromjpeg($arquivo['tmp_name']);
            imagecopyresampled($image_p, $image, 0, 0, 0, 0, $tamanho_x, $tamanho_y, $width, $height);
            imagejpeg($image_p, $caminho, 100);
            //Clean-up memory
            ImageDestroy($image_p);
            //Clean-up memory
            ImageDestroy($image);
            break;
        default:
            if ( !move_uploaded_file( $arquivo['tmp_name'], $caminho ) ) {
                $db->rollback();
                return false;
            } 
    } 
    $db->commit();
    //$db->sucesso("principal/".$dir);
    echo"<script> alert('Operação realizada com sucesso!');</script>";
    echo"<script>window.location.href = 'evento.php?modulo=principal/cadEventoAnexo&acao=A';</script>";
	 
}
*/
/*
function DownloadArquivo($param){
        global $db;
       
        $sql ="SELECT * FROM public.arquivo WHERE arqid = ".$param['arqid'];
        $arquivo = $db->carregar($sql);
        $caminho = APPRAIZ . 'arquivos/'. $_SESSION['sisdiretorio'] .'/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
        if ( !is_file( $caminho ) ) {
            $_SESSION['MSG_AVISO'][] = "Arquivo não encontrado.";
        }
        $filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
        header( 'Content-type: '. $arquivo[0]['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
}
*/
/*
function DeletarDocumento($anexos){
    global $db;
   
   
    $sql = $anexos["sql"];
    $db->executar($sql);
   

    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$anexos["arqid"];
    $db->executar($sql);

    $db->commit();
    echo"<script>window.location.href = 'evento.php?modulo=principal/cadEventoAnexo&acao=A';</script>";
}
*/

/*
if ($_POST){
	salva();
}
*/

?>

<head>
   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>
<?php
//evento.php?modulo=principal/cadEventos&acao=A
$titulo_modulo = "Eventos";
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( $titulo_modulo, 'Solicitar Pré-agendamento de eventos.' );
echo'<br>';

?>
<br>
 <?
 $perfis = arrayPerfil();
 
 if(in_array(PERFIL_EMPRESA, $perfis) &&
 	count($perfis) == 1){
 		
 $res = array();
 	
} else {
	
 $res = array( 
 				0 => array ( "descricao" => "Lista",
						    "id" 		=> "4",
						    "link" 		=> "/evento/evento.php?modulo=inicio&acao=C&submod=evento"
				  		  ),
 				1 => array ( "descricao" => "Informações Básicas",
						    "id" 		=> "4",
						    "link" 		=> "/evento/evento.php?modulo=principal/cadEvento&acao=A"
				  		  )
			);
}	  
if( ( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '') )
{
	array_push($res,
					array ("descricao" => "Documentos Anexos",
							    "id"        => "3",
							    "link" 		=> "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A"
							   ),
		   			array ("descricao" => "Infraestrutura",
							    "id"		=> "2",
							    "link"		=> "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A"
					  		   )
			  );
	
	$pflcod = pegaPerfil( $_SESSION['usucpf'] ); 
/*
	 if( ( $pflcod == PERFIL_SUPER_USUARIO) || ( $pflcod == PERFIL_SAA ) )
	 { 
			array_push($res,	
					array ("descricao" => "Hotéis",
						    "id"		=> "1",
						    "link"		=> "/evento/evento.php?modulo=principal/cadEventoHotel&acao=A"
				  		   ) 
				    );
	 }
*/
	
}


/* RETIRANDO ABA "Estrutura Orçamentária" conforme solicitado na demanda 209333 item 8
     *if($_SESSION['eveid']){
	array_push($res,	
				  	array ("descricao" => "Estrutura Orçamentária",
						    "id"		=> "6",
						    "link"		=> "/evento/evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A"
				  		   )
					  );	
	}
*/

if(mostraAbaDocOS($_SESSION['eveid'])){
	array_push($res,	
				  	array ("descricao" => "Ordem de Serviço",
						    "id"		=> "7",
						    "link"		=> "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A"
				  		   )
					  );	
}
if(mostraAbaDocPagamento($_SESSION['eveid'])){
	array_push($res,	
				  	array ("descricao" => "Documento de Pagamento",
						    "id"		=> "5",
						    "link"		=> "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A"
				  		   )
					  );	
}


if( ( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '') )
{
	array_push($res, 
			  	array ("descricao" => "Avaliação",
					    "id"		=> "0",
					    "link"		=> "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A"
			  		   )
				  );
	
}


echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A");
headEvento($rsDadosEvento[0]['evetitulo'],$rsDadosEvento[0]['everespnome'], $rsDadosEvento[0]['ungdsc'], $rsDadosEvento[0]['ungcod'], $rsDadosEvento[0]['eveurgente'], $rsDadosEvento[0]['evedatainclusao'], false);
echo "<br>";   

//verifica permissão edição
$eventoPermissaoEdicao = eventoPermissaoEdicao();
?>
<form name=formulario id=formulario method=post enctype="multipart/form-data">
<input type="hidden" name="salvar" value="0">
<div style="position:absolute; right:0px;top:80px;">

		</div>
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td valign="top"> 
			<table style="width:100%" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">	
			    <tr>
				        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Arquivo:</td>
			        <td width='50%'>
			            <input type="file" name="arquivo" id="arquivo"/>
			            <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			        </td>      
			    </tr>
			    <tr>
			        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Tipo:</td>
			        <td><?php 
			        if( $pflcod == EVENTO_PERFIL_PERFIL_EMPRESA )
			        {
			        	$sql = "SELECT tpaid AS codigo, tpadescricao AS descricao FROM evento.tipoanexo WHERE tpaid = 5"; 
			        }else{
			        	$sql = "SELECT tpaid AS codigo, tpadescricao AS descricao FROM evento.tipoanexo";
			        }
			          
			        $db->monta_combo('tpaid', $sql, 'S', "Selecione...", '', '', '', '100', 'S', 'tpaid'); 
			        ?></td>
			    </tr>
			    <tr>
			        <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			        <td><?= campo_textarea( 'arqdescricao', 'S', 'S', '', 60, 2, 250 ); ?></td>
			    </tr>   
			    <tr style="background-color: #cccccc">
			        <td>&nbsp;</td>
			        <td><input type="button" name="botao" value="Salvar" onclick="enviar()"></td>
			    </tr> 
			</table>	
			<style>
			.listagem{
				width: 100%;
			}
			</style>		
		    <?php
		    	$acao = "'<center><a href=\"#\" onclick=\"javascript:excluirAnexo(' || arq.arqid || ');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>'"; 
		    	if($eventoPermissaoEdicao) $acao = "''";
		    	
		    	if($_SESSION['eveid']) {
			        $sql = "SELECT
			                        $acao as acao,                       
			                        to_char(arq.arqdata,'DD/MM/YYYY'),
			                        tarq.tpadescricao,
			                        '<a style=\"cursor: pointer; color: blue;\" onclick=\"window.location=\'?modulo=principal/cadEventoAnexo&acao=A&download=S&arqid=' || arq.arqid || '\';\" />' || arq.arqnome || '.'|| arq.arqextensao ||'</a>',
			                        arq.arqtamanho || ' kbs' as tamanho,
			                        arq.arqdescricao,                               
			                        usu.usunome
			                    FROM
			                        ((public.arquivo arq INNER JOIN evento.anexoevento aqb
			                        ON arq.arqid = aqb.arqid) INNER JOIN evento.tipoanexo tarq
			                        ON tarq.tpaid = aqb.tpaid) INNER JOIN seguranca.usuario usu
			                        ON usu.usucpf = arq.usucpf
			                    WHERE
			                        arq.arqstatus = 'A' AND  aqb.eveid = " .$_SESSION['eveid'];
		    	} else {
		    		$sql = array();
		    	}
		
		        $cabecalho = array( "Ação",
		                            "Data Inclusão",
		                            "Tipo Arquivo",
		                            "Nome Arquivo",
		                            "Tamanho (Mb)",
		                            "Descrição Arquivo",
		                            "Responsável");
		        $db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
		    ?>			
		</td>
		<td width="100" align="center">
			<?php wf_desenhaBarraNavegacao( $docid , array( '' => '' ) ); ?>
		</td>
	</tr>
</table>
</form>

<script language="javascript" type="text/javascript">
	function cadastrar_anexo(){
		if ( validar_formulario_anexo() ) {
			document.anexo.submit();
		}
	}
	
	function enviar()
	{
		var arq   = document.getElementById('arquivo');
		var tpaid = document.getElementById('tpaid');
		if( tpaid.value == '' )
		{
			alert('É necessário informar um tipo de documento.');
			return false;
		}
		if( arq.value == '' )
		{
			alert('É necessário anexar um arquivo.');
			return false;
		}
		
		//location.href= window.location+'&salvar=1';
		document.formulario.salvar.value=1;
		document.formulario.submit();
	}
	
	function validar_formulario_anexo(){
		var validacao = true;
		var mensagem = 'Os seguintes campos não foram preenchidos:';
		//document.anexo.taaid.value = trim( document.anexo.taaid.value );
		document.anexo.anedescricao.value = trim( document.anexo.anedescricao.value );
		/*
		if ( document.anexo.taaid.value == '' ) {
			mensagem += '\nTipo';
			validacao = false;
		}
		*/
		if ( document.anexo.anedescricao.value == '' ) {
			mensagem += '\nDescrição';
			validacao = false;
		}
		if ( document.anexo.arquivo.value == '' ) {
			mensagem += '\nArquivo';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
		}
		return validacao;
	}
	
	function excluirAnexo( arqid ){  
		if ( confirm( 'Deseja excluir o Documento?' ) ) {
			location.href= window.location+'&arqidDel='+arqid;
		}
	}
	
	function cadastrar_versao( formulario ){
		if ( formulario.arquivo.value == '' ) {
			return;
		}
		formulario.submit();
	}
	
	function ltrim( value ){
		var re = /\s*((\S+\s*)*)/;
		return value.replace(re, "$1");
	}
	
	function rtrim( value ){
		var re = /((\s*\S+)*)\s*/;
		return value.replace(re, "$1");
	}
	
	function trim( value ){
		return ltrim(rtrim(value));
	}
</script>

<?php 
//verifica permissao edição
echo $eventoPermissaoEdicao;
?>